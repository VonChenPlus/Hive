#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "BASE/Honey.h"
#include "MATH/Rectangle.h"
#include "BASE/HBuffer.h"
#include "MATH/Vector.h"
#include "Encoder.h"

namespace RFB
{
    enum Encoding : uint32
    {
        encodingRaw = 0,
        encodingCopyRect = 1,
        encodingRRE = 2,
        endcodingCoRRE = 4,
        encodingHextile = 5,
        encodingZRLE = 16,
        encodingMax = 255,

        pseudoEncodingCursor = 0xFFFFFF11,
        pseudoEncodingDesktopSize = 0xFFFFFF21
    };

    class DataHandler
    {
    public:
        DataHandler(const HInBuffer &inBuffer)
            : inBuffer_(inBuffer) {
        }
        virtual ~DataHandler() {}

        void setPixelSize(uint64 pixelSize) { pixelSize_ = pixelSize; }
        uint64 getPixelSize() const { return pixelSize_; }

        virtual HInBuffer &getInBuffer() = 0;

        virtual void setDesktopSize(uint16 width, uint16 height) = 0;
        virtual void setCursor(uint16 width, uint16 height, const MATH::Vector2f &hostspot,
                               const HBYTE *data, const HBYTE *mask) = 0;

        virtual void bell() = 0;
        virtual void serverCutText(const std::string &cutText) = 0;

        virtual void beginRect(const MATH::Recti &rect, Encoding encoding) = 0;
        virtual void endRect(const MATH::Recti &rect, Encoding encoding) = 0;
        virtual void copyRect(const MATH::Recti &rect, uint16 srcX, uint16 srcY) = 0;

        virtual void handleImage(const MATH::Recti &block, HBYTE *buffer) = 0;
        virtual void handleFill(const MATH::Recti &block, int buffer) = 0;

    protected:
        uint64 pixelSize_;
        const HInBuffer &inBuffer_;
    };
}

#endif // DATAHANDLER_H
