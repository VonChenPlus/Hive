#ifndef DECODER_H
#define DECODER_H

#include "MATH/HRect.h"
#include "../RFB/DataHandler.h"

namespace RFB
{
    class Decoder
    {
    public:
        Decoder();
        virtual ~Decoder();

        virtual void readRect(const MATH::Rect &block, DataHandler &handle) = 0;

        typedef Decoder* (*DecoderCreateFnType)();
        static Decoder *CraeteDecoder(Encoding encoding);
        static void RegisterDecoder(Encoding encoding, DecoderCreateFnType createFn);

    private:
        static DecoderCreateFnType DecoderCreateFns[encodingMax+1];
    };
}

#endif // DECODER_H
