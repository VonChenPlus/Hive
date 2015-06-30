#include "ProtocolWriter.h"
#include "ProtocolConnection.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"

namespace RFB
{
    ProtocolWriter::ProtocolWriter(ProtocolConnection &connection)
        : connection_(connection) {
    }

    ProtocolWriter::~ProtocolWriter() {
    }

    void ProtocolWriter::initClient(bool shared) {
        HOutBuffer &buffer = connection_.getOutBuffer();
        buffer.writeOne((uint8 *)&shared);
        buffer.flushBuffer();
    }

    void ProtocolWriter::setPixelFormat(PixelFormat &foramt) {
        HOutBuffer &buffer = connection_.getOutBuffer();
        buffer.writeOne((uint8)SetPixelFormat);
        buffer.pad(3);
        foramt.write(buffer);
        buffer.flushBuffer();
    }

    void ProtocolWriter::setEncodings(uint16 nEncodings, uint32 *encodings) {
        HOutBuffer &buffer = connection_.getOutBuffer();
        buffer.writeOne((uint8)SetPixelFormat);
        buffer.pad(1);
        buffer.writeOne(&nEncodings);
        for (uint16 index = 0; index < nEncodings; ++index) {
            buffer.writeOne(encodings[index]);
        }
        buffer.flushBuffer();
    }
}
