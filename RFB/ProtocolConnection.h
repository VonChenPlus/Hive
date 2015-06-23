#ifndef PROTOCOLCONNECTION_H
#define PROTOCOLCONNECTION_H

#include <vector>

#include "BASE/Native.h"
#include "IO/TCPSocket.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"
#include "ProtocolInfo.h"
#include "ProtocolSecurity.h"

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

        NInBuffer &getInBuffer() { return inBuffer_; }
        NOutBuffer &getOutBuffer() { return outBuffer_; }

    private:
        void initialize();

        void addSecType(SecurityType secType);
        ProtocolSecurity *getSecHandler(SecurityType secType);

        void processVersion();
        void processSecurityTypes();

    private:
        bool closed_;
        IO::TCPSocket sock_;
        IO::FDInBuffer inBuffer_;
        IO::FDOutBuffer outBuffer_;
        std::string serverName_;

        std::vector<SecurityType> secTypes_;
        ProtocolSecurity *securityHandler_;

        bool useProtocol3_3;
        ProtocolState protocolState_;
        ProtocolInfo protocolInfo_;
    };
}

#endif // PROTOCOLCONNECTION_H
