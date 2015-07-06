#include "Decoder.h"
#include "RawDecoder.h"
#include "RREDecoder.h"
#include "HextileDecoder.h"
#include "ZRLEDecoder.h"

namespace RFB
{
    Decoder::DecoderCreateFnType Decoder::DecoderCreateFns[encodingMax+1] = { 0 };

    Decoder::Decoder() {
    }

    Decoder::~Decoder() {
    }

    Decoder *Decoder::CraeteDecoder(Encoding encoding) {
        if (encoding < encodingMax && DecoderCreateFns[encoding]) {
            return DecoderCreateFns[encoding]();
        }

        throw _HException_Normal("CraeteDecoder Failed!");
    }

    void Decoder::RegisterDecoder(Encoding encoding, DecoderCreateFnType createFn) {
        if (encoding > encodingMax) {
            throw _HException_Normal("Decoder::registerDecoder: encoding out of range");
        }

        DecoderCreateFns[encoding] = createFn;
    }

    class DecoderInit
    {
    public:
        DecoderInit() {
            Decoder::RegisterDecoder(encodingRaw, RawDecoder::Create);
            Decoder::RegisterDecoder(encodingRRE, RREDecoder::Create);
            Decoder::RegisterDecoder(encodingHextile, HextileDecoder::Create);
            Decoder::RegisterDecoder(encodingZRLE, ZRLEDecoder::Create);
        }
    };

    DecoderInit decoderInit;
}
