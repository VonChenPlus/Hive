#include "ProtocolReader.h"
#include "ProtocolConnection.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"

namespace RFB
{
    ProtocolReader::ProtocolReader(ProtocolConnection &connection)
        : connection_(connection) {
    }

    ProtocolReader::~ProtocolReader() {
    }

    void ProtocolReader::readServerInitInfo() {
        HInBuffer &buffer = connection_.getInBuffer();
        ProtocolInfo &info = connection_.getProtocolInfo();

        uint16 width = buffer.readOne<uint16>();
        uint16 height = buffer.readOne<uint16>();
        info.setDesktopSize(width, height);
        PixelFormat pixelFormat;
        pixelFormat.read(buffer);
        info.setPixelFormat(pixelFormat);

        uint32 length = buffer.readOne<uint32>();
        std::string name;
        name.resize(length + 1);
        buffer.readAny(length, &name[0]);
        info.setName(name);
    }
}
