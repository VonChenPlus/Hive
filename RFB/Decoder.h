#ifndef DECODER_H
#define DECODER_H

#include "MATH/NRect.h"
#include "../RFB/DataHandler.h"

namespace RFB
{
    class Decoder
    {
    public:
        Decoder() {}
        virtual ~Decoder() {}

        virtual void readRect(const MATH::Rect &block, DataHandler &handle) = 0;
    };
}

#endif // DECODER_H
