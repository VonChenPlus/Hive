#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "BASE/Honey.h"
#include "MATH/HRect.h"
#include "BASE/HBuffer.h"
#include "MATH/Point.h"
#include "Encoder.h"

namespace RFB
{
    class DataHandler
    {
    public:
        DataHandler(const HInBuffer &inBuffer)
            : inBuffer_(inBuffer) {
        }
        virtual ~DataHandler() {}

        void setPixelSize(Size pixelSize) { pixelSize_ = pixelSize; }
        Size getPixelSize() const { return pixelSize_; }

        virtual HInBuffer &getInBuffer() = 0;

        virtual void setDesktopSize(uint16 width, uint16 height) = 0;
        virtual void setCursor(uint16 width, uint16 height, const MATH::Point &hostspot,
                               const HBYTE *data, const HBYTE *mask) = 0;

        virtual void beginRect(const MATH::Rect &rect, Encoding encoding) = 0;
        virtual void endRect(const MATH::Rect &rect, Encoding encoding) = 0;
        virtual void copyRect(const MATH::Rect &rect, uint16 srcX, uint16 srcY) = 0;

        virtual void handleImage(const MATH::Rect &block, HBYTE *buffer) = 0;
        virtual void handleFill(const MATH::Rect &block, int buffer) = 0;

    protected:
        size_t pixelSize_;
        const HInBuffer &inBuffer_;
    };
}

#endif // DATAHANDLER_H
