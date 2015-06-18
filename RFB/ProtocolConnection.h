#ifndef PROTOCOLCONNECTION_H
#define PROTOCOLCONNECTION_H

#include <vector>

#include "BASE/Native.h"
#include "IO/TCPSocket.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"
#include "ProtocolInfo.h"

namespace RFB
{
    enum SecurityType
    {
        secTypeInvalid = 0,

        secTypeNone = 1,
        secTypeVncAuth = 2,

        secTypeRA2 = 5,
        secTypeRA2ne = 6,

        secTypeSSPI = 7,
        secTypeSSPIne = 8,

        secTypeTight = 16,
        secTypeUltra = 17,
        secTypeTLS = 18
    };

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

    private:
        void initialise();

        void addSecType(SecurityType secType);

        void processVersion();
        void processSecurityTypes();

    private:
        bool closed_;
        IO::TCPSocket sock_;
        IO::FDInBuffer inBuffer_;
        IO::FDOutBuffer outBuffer_;
        std::string serverName_;

        std::vector<SecurityType> secTypes_;
        bool useProtocol3_3;
        ProtocolState protocolState_;
        ProtocolInfo protocolInfo_;
    };
}

#endif // PROTOCOLCONNECTION_H
