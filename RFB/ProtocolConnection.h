#ifndef PROTOCOLCONNECTION_H
#define PROTOCOLCONNECTION_H

#include <vector>
#include <memory>

#include "BASE/Honey.h"
#include "IO/TCPSocket.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"
#include "ProtocolInfo.h"
#include "ProtocolSecurity.h"
#include "ProtocolWriter.h"
#include "ProtocolReader.h"

namespace RFB
{
    enum ProtocolState
    {
        UNINITIALISED,
        PROTOCOL_VERSION,
        SECURITY_TYPES,
        SECURITY,
        SECURITY_RESULT,
        INITIALISATION,
        NORMAL,
        INVALID
    };

    class ProtocolConnection
    {
    public:
        ProtocolConnection(const char *host, int port);
        ~ProtocolConnection();

        void process();
        bool isCloseed() { return closed_; }

        HInBuffer &getInBuffer() { return inBuffer_; }
        HOutBuffer &getOutBuffer() { return outBuffer_; }

        ProtocolInfo &getProtocolInfo() { return protocolInfo_; }

    private:
        void initialize();

        void addSecType(SecurityType secType);
        std::shared_ptr<ProtocolSecurity> getSecHandler(SecurityType secType);

        void processVersion();
        void processSecurityTypes();
        void processSecurity();
        void processSecurityResult();
        void processInit();

    private:
        bool useProtocol3_3;
        bool shared_;

        bool closed_;
        IO::TCPSocket sock_;
        IO::FDInBuffer inBuffer_;
        IO::FDOutBuffer outBuffer_;
        std::string serverName_;

        std::vector<SecurityType> secTypes_;
        std::shared_ptr<ProtocolSecurity> securityHandler_;

        std::shared_ptr<ProtocolWriter> writer_;
        std::shared_ptr<ProtocolReader> reader_;

        ProtocolState protocolState_;
        ProtocolInfo protocolInfo_;
    };
}

#endif // PROTOCOLCONNECTION_H
