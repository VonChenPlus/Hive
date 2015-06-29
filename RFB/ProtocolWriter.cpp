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
        connection_.getOutBuffer().writeOne((uint8 *)&shared);
        connection_.getOutBuffer().flushBuffer();
    }
}
