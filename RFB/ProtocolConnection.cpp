#include "ProtocolConnection.h"
using IO::TCPSocket;
#include "UTILS/STRING/HString.h"
using UTILS::STRING::StringFromFormat;

namespace RFB
{
    ProtocolConnection::ProtocolConnection(const char *host, int port, DataHandler &dataHandler)
        : closed_(false)
        , sock_(host, port)
        , inBuffer_(sock_.getSock())
        , outBuffer_(sock_.getSock())
        , dataHandler_(dataHandler) {
        if (!TCPSocket::isConnected(sock_.getSock())) {
            throw _HException_Normal("RFB Protocol Connect Failed!");
        }
        initialize();
    }

    ProtocolConnection::~ProtocolConnection() {
    }

    void ProtocolConnection::process() {
        switch (protocolState_) {
        case PROTOCOL_VERSION:
            processVersion();
            break;
        case SECURITY_TYPES:
            processSecurityTypes();
            break;
        case SECURITY:
            processSecurity();
            break;
        case SECURITY_RESULT:
            processSecurityResult();
            break;
        case INITIALISATION:
            processInit();
            break;
        case NORMAL:
            reader_->readMessage();
            break;
        case UNINITIALISED:
            throw _HException_Normal("Protocol not initialised yet!");
        default:
            throw _HException_Normal("Invalid Protocol State!");
        }
    }

    void ProtocolConnection::initialize() {
        serverName_ = sock_.getPeerEndpoint();
        useProtocol3_3 = false;
        shared_ = false;

        // - Set which auth schemes we support, in order of preference
        addSecType(secTypeVncAuth);
        addSecType(secTypeNone);

        protocolState_ = PROTOCOL_VERSION;
    }

    void ProtocolConnection::addSecType(SecurityType secType) {
        secTypes_.push_back(secType);
    }

    std::shared_ptr<ProtocolSecurity> ProtocolConnection::getSecHandler(SecurityType secType) {
        switch (secType) {
          case secTypeNone:
            return std::make_shared<ProtocolSecurityNone>();
          case secTypeVncAuth:
            return std::make_shared<ProtocolSecurityVncAuth>((UserPasswdGetter *)NULLPTR);
          default:
            throw _HException_Normal("Unsupported secType?");
        }
    }

    void ProtocolConnection::processVersion() {
        // Protocol Version Length is 12
        std::string protocolVersion;
        protocolVersion.resize(12);
        inBuffer_.readAny(12, &protocolVersion[0]);
        protocolInfo_.setVersion(protocolVersion);

        // The only official RFB protocol versions are currently 3.3, 3.7 and 3.8
        if (protocolInfo_.beforeVersion(3, 3)) {
            protocolState_ = INVALID;
            throw _HException_Normal(StringFromFormat("Server gave unsupported RFB protocol version %d.%d",
                                     protocolInfo_.majorVersion(),
                                     protocolInfo_.minorVerison()));
        }
        else if (useProtocol3_3 || protocolInfo_.beforeVersion(3, 7)) {
            protocolInfo_.setVersion(3, 3);
        }
        else if (protocolInfo_.afterVersion(3, 8)) {
            protocolInfo_.setVersion(3, 8);
        }

        std::string rfbVersion = StringFromFormat("RFB %03d.%03d\n",
                                                  protocolInfo_.majorVersion(),
                                                  protocolInfo_.minorVerison());
        outBuffer_.writeAny(rfbVersion.size(), rfbVersion.c_str());
        outBuffer_.flushBuffer(rfbVersion.size());

        protocolState_ = SECURITY_TYPES;
    }

    void ProtocolConnection::processSecurityTypes() {
        SecurityType secType = secTypeInvalid;

        // legacy 3.3 server may only offer "vnc authentication" or "none"
        if (protocolInfo_.isVersion(3, 3)) {
            inBuffer_.readAny(sizeof(int), &secType);
            if (secType == secTypeInvalid) {
                protocolState_ = INVALID;
                throw _HException_Normal("RFB Secure Type Error!");
            }
            else if (secType == secTypeNone || secType == secTypeVncAuth) {
                bool validType = false;
                for (const auto &iter : secTypes_) {
                    if (iter == secType) {
                        validType = true;
                        break;
                    }
                }
                if (!validType)
                    secType = secTypeInvalid;
            }
            else {
                throw _HException_Normal("Unknown 3.3 RFB Secure Type!");
            }
        }
        // >=3.7 server will offer us a list
        else {
            int nServerSecTypes = 0;
            inBuffer_.readAny(sizeof(uint8), &nServerSecTypes);
            if (nServerSecTypes == 0) {
                protocolState_ = INVALID;
                throw _HException_Normal("RFB Secure Type Error!");
            }

            // If we haven't already chosen a secType, try this one
            // If we are using the client's preference for types,
            // we keep trying types, to find the one that matches and
            // which appears first in the client's list of supported types.
            for (int index = 0; index < nServerSecTypes; ++index) {
                SecurityType serverSecType = secTypeInvalid;
                inBuffer_.readAny(sizeof(uint8), &serverSecType);
                for (const auto &iter : secTypes_) {
                    if (iter == serverSecType) {
                        secType = serverSecType;
                        break;
                    }
                }
            }

            // Inform the server of our decision
            if (secType != secTypeInvalid) {
                outBuffer_.writeAny(sizeof(uint8), &secType);
                outBuffer_.flushBuffer();
            }
        }

        if (secType == secTypeInvalid) {
            protocolState_ = INVALID;
            throw _HException_Normal("RFB No matching security types!");
        }

        protocolState_ = SECURITY;
        securityHandler_ = getSecHandler(secType);
    }

    void ProtocolConnection::processSecurity() {
        if (securityHandler_ && securityHandler_->process(*this)) {
            protocolState_ = SECURITY_RESULT;
            return;
        }

        throw _HException_Normal("Security Handler Error!");
    }

    void ProtocolConnection::processSecurityResult() {
        SecurityResult result = secResultUnknown;
        if (protocolInfo_.beforeVersion(3, 8) && securityHandler_->getType() == secTypeNone) {
            result = secResultOK;
        }
        else {
            inBuffer_.readAny(sizeof(uint32), &result);
        }

        switch (result) {
        case secResultOK:
            protocolState_ = INITIALISATION;
            break;
        case secResultFailed:
        case secResultTooMany:
        default:
            std::string errorMsg;
            if (protocolInfo_.beforeVersion(3, 8)) {
                errorMsg = "Authentication failure";
            }
            else {
                uint32 errorLen = 0;
                inBuffer_.readOne(&errorLen);
                errorMsg.resize(errorLen + 1);
                inBuffer_.readAny(errorLen, &errorMsg[0]);
            }
            protocolState_ = INVALID;
            throw _HException_Normal(errorMsg);
        }
    }

    void ProtocolConnection::processInit() {
        writer_ = std::make_shared<ProtocolWriter>(*this);
        reader_ = std::make_shared<ProtocolReader>(*this);
        writer_->writeInitClient(shared_);
        reader_->readServerInitInfo();
        protocolState_ = NORMAL;
    }
}
