#include "ProtocolInfo.h"

namespace RFB
{
    ProtocolInfo::ProtocolInfo()
        : majorVersion_(0)
        , minorVersion_(0) {

    }

    ProtocolInfo::~ProtocolInfo() {

    }

    void ProtocolInfo::setVersion(const std::string &version) {
#ifdef WIN32
        sscanf_s(version.c_str(), "RFB %03d.%03d\n",
                 &majorVersion_,
                 &minorVersion_);
#else
        sscanf(version.c_str(), "RFB %03d.%03d\n",
               &majorVersion_,
               &minorVersion_);
#endif
    }

    void ProtocolInfo::setVersion(int major, int minor) {
        majorVersion_ = major;
        minorVersion_ = minor;
    }

    bool ProtocolInfo::isVersion(int major, int minor) {
        return majorVersion_ == major && minorVersion_ == minor;
    }

    bool ProtocolInfo::beforeVersion(int major, int minor) {
        return (majorVersion_ < major
                || (majorVersion_ == major && minorVersion_ < minor));
    }

    bool ProtocolInfo::afterVersion(int major, int minor) {
        return !beforeVersion(major, minor);
    }

    void ProtocolInfo::setDesktopSize(uint16 width, uint16 height) {
        width_ = width;
        height_ = height;
    }
}

