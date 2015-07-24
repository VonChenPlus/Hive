#include "ProtocolWriter.h"
#include "ProtocolConnection.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"
using MATH::Vector2i;
using MATH::Recti;

namespace RFB
{
    ProtocolWriter::ProtocolWriter(ProtocolConnection &connection)
        : connection_(connection) {
    }

    ProtocolWriter::~ProtocolWriter() {
    }

    void ProtocolWriter::writeInitClient(bool shared) {
        HOutBuffer &buffer = connection_.getOutBuffer();

        buffer.writeOne((uint8 *)&shared);
        buffer.flushBuffer();
    }

    void ProtocolWriter::writeSetPixelFormat(PixelFormat &foramt) {
        HOutBuffer &buffer = connection_.getOutBuffer();

        buffer.writeOne(static_cast<uint8>(SetPixelFormat));
        buffer.pad(3);
        foramt.write(buffer);
        buffer.flushBuffer();
    }

    void ProtocolWriter::writeSetEncodings(Encoding preferredEncoding, bool useCopyRect) {
        ProtocolInfo &protocolInfo = connection_.getProtocolInfo();

        int nEncodings = 0;
        Encoding encodings[encodingMax + 2];
        if (protocolInfo.supportsLocalCursor()) {
            encodings[nEncodings++] = pseudoEncodingCursor;
        }
        if (protocolInfo.supportsDesktopResize()) {
            encodings[nEncodings++] = pseudoEncodingDesktopSize;
        }
        if (Decoder::Supported(preferredEncoding)) {
            encodings[nEncodings++] = preferredEncoding;
        }
        if (useCopyRect) {
            encodings[nEncodings++] = encodingCopyRect;
        }
        for (int index = encodingMax; index >=0; index--) {
            Encoding curEncoding = static_cast<Encoding>(index);
            if (curEncoding != preferredEncoding && Decoder::Supported(curEncoding)) {
                encodings[nEncodings++] = curEncoding;
            }
        }

        writeSetEncodings(nEncodings, encodings);
    }

    void ProtocolWriter::writeFramebufferUpdateRequest(const MATH::Recti& rect, bool incremental) {
        HOutBuffer &buffer = connection_.getOutBuffer();

        buffer.writeOne(static_cast<uint8>(FramebufferUpdateRequest));
        buffer.writeOne(static_cast<uint8>(incremental));
        buffer.writeOne(static_cast<uint16>(rect.minX()));
        buffer.writeOne(static_cast<uint16>(rect.minY()));
        buffer.writeOne(static_cast<uint16>(rect.width()));
        buffer.writeOne(static_cast<uint16>(rect.height()));
    }

    void ProtocolWriter::writeKeyEvent(uint32 key, bool down) {
        HOutBuffer &buffer = connection_.getOutBuffer();

        buffer.writeOne(static_cast<uint8>(KeyEvent));
        buffer.writeOne(static_cast<uint8>(down));
        buffer.pad(2);
        buffer.writeOne(key);
        buffer.flushBuffer();
    }

    void ProtocolWriter::writeMouseEvent(const Vector2i &pos, uint8 buttonMask) {
        HOutBuffer &buffer = connection_.getOutBuffer();
        ProtocolInfo &protocolInfo = connection_.getProtocolInfo();

        Vector2i fixedPos;
        Recti desktipSize = protocolInfo.getDesktopSize();
        Vector2i::clamp(pos, Vector2i(0, desktipSize.width()), Vector2i(0, desktipSize.height()), fixedPos);
        buffer.writeOne(static_cast<uint8>(PointerEvent));
        buffer.writeOne(buttonMask);
        buffer.writeOne(static_cast<uint16>(fixedPos.x));
        buffer.writeOne(static_cast<uint16>(fixedPos.y));
        buffer.flushBuffer();
    }

    void ProtocolWriter::writeSetEncodings(uint16 nEncodings, Encoding *encodings) {
        HOutBuffer &buffer = connection_.getOutBuffer();

        buffer.writeOne(static_cast<uint8>(SetEncodings));
        buffer.pad(1);
        buffer.writeOne(&nEncodings);
        for (uint16 index = 0; index < nEncodings; ++index) {
            buffer.writeOne(encodings[index]);
        }
        buffer.flushBuffer();
    }

    void ProtocolWriter::writeClientCutText(const std::string &cutText) {
        HOutBuffer &buffer = connection_.getOutBuffer();

        buffer.writeOne(static_cast<uint8>(ClientCutText));
        buffer.pad(3);
        buffer.writeOne(static_cast<uint32>(cutText.length()));
        buffer.writeAny(cutText.length(), &cutText[0]);
        buffer.flushBuffer();
    }
}
