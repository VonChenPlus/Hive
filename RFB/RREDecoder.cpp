#include "RREDecoder.h"

namespace RFB
{
    Decoder* RREDecoder::Create() {
        return new RREDecoder();
    }

    RREDecoder::RREDecoder() {

    }

    RREDecoder::~RREDecoder() {

    }

    void RREDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        switch (handle.getPixelSize()) {
            case 8:
                rreDecode<uint8>(block, handle.getInBuffer(), handle);
                break;
            case 16:
                rreDecode<uint16>(block, handle.getInBuffer(), handle);
                break;
            case 32:
            default:
                rreDecode<uint32>(block, handle.getInBuffer(), handle);
                break;
        }
    }
}

