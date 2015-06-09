#include "ProtocolConnection.h"
using IO::TCPSocket;

namespace RFB
{
    ProtocolConnection::ProtocolConnection(const char *host, int port)
        : closed_(false)
        , sock_(host, port)
        , buffer_(sock_.getSock()) {
        if (!TCPSocket::isConnected(sock_.getSock())) {
            throw _NException_Normal("RFB Protocol Connect Failed!");
        }

        initialise();
    }

    ProtocolConnection::~ProtocolConnection() {

    }

    void ProtocolConnection::process() {
        switch (protocolState_) {
        case PROTOCOL_VERSION:
            processVersion();
            break;
        case SECURITY_TYPES:
            break;
        case SECURITY:
            break;
        case SECURITY_RESULT:
            break;
        case INITIALISATION:
            break;
        case NORMAL:
            break;
        case UNINITIALISED:
            throw _NException_Normal("Protocol not initialised yet!");
        default:
            throw _NException_Normal("Invalid Protocol State!");
        }
    }

    void ProtocolConnection::initialise() {
        serverName_ = sock_.getPeerEndpoint();
        useProtocol3_3 = true;

        // - Set which auth schemes we support, in order of preference
        addSecType(secTypeVncAuth);
        addSecType(secTypeNone);

        protocolState_ = PROTOCOL_VERSION;
    }

    void ProtocolConnection::addSecType(SecurityType secType) {
        secTypes_.push_back(secType);
    }

    void ProtocolConnection::processVersion() {
        // Protocol Version Length is 12
        std::string protocolVersion;
        protocolVersion.resize(12);
        buffer_.take(12, &protocolVersion[0]);
        protocolInfo_.setVersion(protocolVersion);
    }
}
