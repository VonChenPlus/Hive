#include "RREDecoder.h"

namespace RFB
{
    RREDecoder::RREDecoder() {

    }

    RREDecoder::~RREDecoder() {

    }

    void RREDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        switch (handle.pixelSize()) {
            case 8:
                rreDecode<uint8>(block, handle.getBuffer(), handle);
                break;
            case 16:
                rreDecode<uint16>(block, handle.getBuffer(), handle);
                break;
            case 32:
            default:
                rreDecode<uint32>(block, handle.getBuffer(), handle);
                break;
        }
    }
}

