#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "BASE/Native.h"
#include "MATH/NRect.h"
#include "BASE/NBuffer.h"

namespace RFB
{
    class DataHandler
    {
    public:
        DataHandler(size_t pixelSize) { pixelSize_ = pixelSize; }
        virtual ~DataHandler() {}

        virtual NInBuffer &getBuffer() const = 0;
        virtual void handleImage(const MATH::Rect &block, NBYTE *buffer) = 0;
        virtual void handleFill(const MATH::Rect &block, int buffer) = 0;
        size_t pixelSize() const { return pixelSize_; }

    protected:
        size_t pixelSize_;
    };
}

#endif // DATAHANDLER_H
