#include "ProtocolReader.h"
#include "ProtocolConnection.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"
#include "Encoder.h"
#include "UTILS/STRING/StringUtils.h"
using UTILS::STRING::StringFromFormat;

namespace RFB
{
    ProtocolReader::ProtocolReader(ProtocolConnection &connection)
        : connection_(connection)
        , updateRectsLeft_(0) {

    }

    ProtocolReader::~ProtocolReader() {
    }

    void ProtocolReader::readServerInitInfo() {
        HInBuffer &buffer = connection_.getInBuffer();
        ProtocolInfo &protocolInfo = connection_.getProtocolInfo();

        uint16 width = buffer.readOne<uint16>();
        uint16 height = buffer.readOne<uint16>();
        protocolInfo.setDesktopSize(width, height);
        PixelFormat pixelFormat;
        pixelFormat.read(buffer);
        protocolInfo.setPixelFormat(pixelFormat);

        uint32 length = buffer.readOne<uint32>();
        std::string name;
        name.resize(length + 1);
        buffer.readAny(length, &name[0]);
        protocolInfo.setName(name);
    }

    void ProtocolReader::readMessage() {
        HInBuffer &buffer = connection_.getInBuffer();
        ProtocolInfo &protocolInfo = connection_.getProtocolInfo();

        if (updateRectsLeft_ == 0) {
            MessageForRead type = static_cast<MessageForRead>(buffer.readOne<uint8>());
            switch (type) {
            case FramebufferUpdate :
                readFramebufferUpdate();
                break;
            case SetColorMapEntries:
                readSetColorMapEntries();
                break;
            case Bell:
                readBell();
                break;
            case ServerCutText:
                readServerCutText();
                break;
            default:
                throw _HException_Normal("Unknowd message Type");
            }
        }
        else {
            uint16 xPos = buffer.readOne<uint16>();
            uint16 yPos = buffer.readOne<uint16>();
            uint16 width = buffer.readOne<uint16>();
            uint16 height = buffer.readOne<uint16>();
            Encoding encoding = static_cast<Encoding>(buffer.readOne<uint32>());

            switch (encoding) {
            case pseudoEncodingDesktopSize:
                protocolInfo.setDesktopSize(width, height);
                break;
            case pseudoEncodingCursor:
                readCursor(width, height, MATH::Vector2f(xPos, yPos));
                break;
            default:
                readRect(MATH::Recti(xPos, yPos, width, height), encoding);
            }
        }
    }

    void ProtocolReader::readFramebufferUpdate() {
        HInBuffer &buffer = connection_.getInBuffer();

        buffer.skip(1);
        updateRectsLeft_ = buffer.readOne<uint16>();
    }

    void ProtocolReader::readSetColorMapEntries() {
        throw _HException_Normal("setColourMapEntries called");
    }

    void ProtocolReader::readBell() {
        DataHandler &dataHandler = connection_.getDataHandler();

        dataHandler.bell();
    }

    void ProtocolReader::readServerCutText() {
        HInBuffer &buffer = connection_.getInBuffer();
        DataHandler &dataHandler = connection_.getDataHandler();

        buffer.skip(3);
        uint32 length = buffer.readOne<uint32>();
        if (length > 256 * 1024) {
            buffer.skip(length);
            throw _HException_Normal("cut text too long - ignoring\n");
        }
        else {
            std::string cutText;
            cutText.resize(length + 1);
            buffer.readAny(length, &cutText[0]);
            dataHandler.serverCutText(cutText);
        }
    }

    void ProtocolReader::readCursor(uint16 width, uint16 height, const MATH::Vector2f &hostpot) {
        HInBuffer &buffer = connection_.getInBuffer();
        DataHandler &dataHandler = connection_.getDataHandler();
        ProtocolInfo &protocolInfo = connection_.getProtocolInfo();

        Size dataLength = width * height * protocolInfo.getPixelFormat().bitsPer() / 8;
        Size maskLength = (width + 7) / 8 * height;
        std::string data; data.resize(dataLength + 1);
        std::string mask; mask.resize(maskLength + 1);
        buffer.readAny(dataLength, &data[0]);
        buffer.readAny(maskLength, &mask[0]);
        dataHandler.setCursor(width, height, hostpot, data.c_str(), mask.c_str());
    }

    void ProtocolReader::readRect(const MATH::Recti rect, Encoding encoding) {
        ProtocolInfo &protocolInfo = connection_.getProtocolInfo();
        DataHandler &dataHandler = connection_.getDataHandler();
        MATH::Recti desktopSize = protocolInfo.getDesktopSize();

        if (rect.empty())
            throw _HException_Normal("Warning: zero size rect");
        if (rect.maxX() > desktopSize.width() || rect.maxY() > desktopSize.height()) {
            throw _HException_Normal(StringFromFormat("Rect too big: %dx%d at %d,%d exceeds %dx%d\n",
                                     rect.width(), rect.height(), rect.minX(), rect.minY(),
                                     desktopSize.width(), desktopSize.height()));
        }

        dataHandler.beginRect(rect, encoding);

        switch (encoding) {
        case encodingCopyRect:
            readCopyRect(rect);
            break;
        default:
            if (encoding > encodingMax)
                throw _HException_Normal("Unknown rect encoding");
            if (!decoder_[encoding])
                decoder_[encoding] = Decoder::CraeteDecoder(encoding);
            decoder_[encoding]->readRect(rect, dataHandler);
            break;
        }

        dataHandler.endRect(rect, encoding);
    }

    void ProtocolReader::readCopyRect(const MATH::Recti rect) {
        HInBuffer &buffer = connection_.getInBuffer();
        DataHandler &dataHandler = connection_.getDataHandler();

        uint16 srcX = buffer.readOne<uint16>();
        uint16 srcY = buffer.readOne<uint16>();
        dataHandler.copyRect(rect, srcX, srcY);
    }
}
