#include "ProtocolInfo.h"

namespace RFB
{
    ProtocolInfo::ProtocolInfo() {

    }

    ProtocolInfo::~ProtocolInfo() {

    }

    void ProtocolInfo::setVersion(const std::string &version) {
        version_ = version;
    }
}

