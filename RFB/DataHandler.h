#ifndef DATAHANDLER_H
#define DATAHANDLER_H

#include "BASE/Honey.h"
#include "MATH/NRect.h"
#include "BASE/HBuffer.h"

namespace RFB
{
    class DataHandler
    {
    public:
        DataHandler(size_t pixelSize) { pixelSize_ = pixelSize; }
        virtual ~DataHandler() {}

        virtual HInBuffer &getBuffer() const = 0;
        virtual void handleImage(const MATH::Rect &block, HBYTE *buffer) = 0;
        virtual void handleFill(const MATH::Rect &block, int buffer) = 0;
        size_t pixelSize() const { return pixelSize_; }

    protected:
        size_t pixelSize_;
    };
}

#endif // DATAHANDLER_H
