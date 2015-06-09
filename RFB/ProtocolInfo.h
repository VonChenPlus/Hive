#ifndef PROTOCOLINFO_H
#define PROTOCOLINFO_H

#include "BASE/Native.h"

namespace RFB
{
    class ProtocolInfo
    {
    public:
        ProtocolInfo();
        ~ProtocolInfo();

        void setVersion(const std::string &version);

    private:
        std::string version_;
    };
}

#endif // PROTOCOLINFO_H
