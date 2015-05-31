#include "ZRLEDecoder.h"

namespace RFB
{
    ZRLEDecoder::ZRLEDecoder() {
    }

    ZRLEDecoder::~ZRLEDecoder() {
    }

    void ZRLEDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        NBYTE buffer[16 * 16 * 4];
        switch (handle.pixelSize()) {
            case 8:
                ZRLEDecode<uint8>(block, handle.getBuffer(), buffer, handle);
                break;
            case 16:
                ZRLEDecode<uint16>(block, handle.getBuffer(), buffer, handle);
                break;
            case 24:
                throw _NException_Normal("Unsupport ZRLEDecoder pixelSize");
            case 32:
            default:
                ZRLEDecode<uint32>(block, handle.getBuffer(), buffer, handle);
                break;
        }
    }
}
