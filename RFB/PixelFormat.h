#ifndef PIXELFORMAT_H
#define PIXELFORMAT_H

#include "BASE/Honey.h"
#include "IO/FDInBuffer.h"
#include "IO/FDOutBuffer.h"

namespace RFB
{
    class PixelFormat
    {
    public:
        PixelFormat();
        ~PixelFormat();

        bool equal(const PixelFormat& other) const;

        void read(HInBuffer &buffer);
        void write(HOutBuffer &buffer);

    private:
        uint8 bitsPer_;
        uint8 depth_;
        uint8 bigEndian_;
        uint8 trueColor_;
        uint16 redMax_;
        uint16 greenMax_;
        uint16 blueMax_;
        uint8 redShift_;
        uint8 greenShift_;
        uint8 blueShift_;
    };
}

#endif // PIXELFORMAT_H
