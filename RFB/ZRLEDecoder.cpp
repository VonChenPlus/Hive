#include "ZRLEDecoder.h"

namespace RFB
{
    ZRLEDecoder::ZRLEDecoder() {
    }

    ZRLEDecoder::~ZRLEDecoder() {
    }

    void ZRLEDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        HBYTE buffer[16 * 16 * 4];
        switch (handle.getPixelSize()) {
            case 8:
                ZRLEDecode<uint8>(block, handle.getInBuffer(), buffer, handle);
                break;
            case 16:
                ZRLEDecode<uint16>(block, handle.getInBuffer(), buffer, handle);
                break;
            case 24:
                throw _HException_Normal("Unsupport ZRLEDecoder pixelSize");
            case 32:
            default:
                ZRLEDecode<uint32>(block, handle.getInBuffer(), buffer, handle);
                break;
        }
    }
}
