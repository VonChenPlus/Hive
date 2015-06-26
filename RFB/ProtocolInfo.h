#ifndef PROTOCOLINFO_H
#define PROTOCOLINFO_H

#include "BASE/Honey.h"

namespace RFB
{
    class ProtocolInfo
    {
    public:
        ProtocolInfo();
        ~ProtocolInfo();

        void setVersion(const std::string &version);
        void setVersion(int major, int minor);

        bool isVersion(int major, int minor);
        bool beforeVersion(int major, int minor);
        bool afterVersion(int major, int minor);

        int majorVersion() { return majorVersion_; }
        int minorVerison() { return minorVersion_; }

    private:
        int majorVersion_;
        int minorVersion_;
    };
}

#endif // PROTOCOLINFO_H
