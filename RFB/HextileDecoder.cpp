#include "HextileDecoder.h"

#include <memory>

namespace RFB
{
    Decoder* HextileDecoder::Create() {
        return new HextileDecoder();
    }

    HextileDecoder::HextileDecoder() {
    }

    HextileDecoder::~HextileDecoder() {
    }

    void HextileDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        HBYTE buffer[16 * 16 * 4];
        switch (handle.getPixelSize()) {
            case 8:
                hextileDecode<uint8>(block, handle.getInBuffer(), buffer, handle);
                break;
            case 16:
                hextileDecode<uint16>(block, handle.getInBuffer(), buffer, handle);
                break;
            case 32:
            default:
                hextileDecode<uint32>(block, handle.getInBuffer(), buffer, handle);
                break;
        }
    }
}
