#include "HextileDecoder.h"

#include <memory>

namespace RFB
{
    HextileDecoder::HextileDecoder() {
    }

    HextileDecoder::~HextileDecoder() {
    }

    void HextileDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        NBYTE buffer[16 * 16 * 4];
        switch (handle.pixelSize()) {
            case 8:
                hextileDecode<uint8>(block, handle.getBuffer(), buffer, handle);
                break;
            case 16:
                hextileDecode<uint16>(block, handle.getBuffer(), buffer, handle);
                break;
            case 32:
            default:
                hextileDecode<uint32>(block, handle.getBuffer(), buffer, handle);
                break;
        }
    }
}
