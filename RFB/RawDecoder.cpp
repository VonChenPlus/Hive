#include "RawDecoder.h"

#include <memory>

namespace RFB
{
    RawDecoder::RawDecoder() {

    }

    RawDecoder::~RawDecoder() {

    }

    void RawDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        NBYTE buffer[16 * 16 * 4];
        switch (handle.pixelSize()) {
            case 8:
                rawDecode<uint8>(block, handle.getBuffer(), buffer, handle);
                break;
            case 16:
                rawDecode<uint16>(block, handle.getBuffer(), buffer, handle);
                break;
            case 32:
            default:
                rawDecode<uint32>(block, handle.getBuffer(), buffer, handle);
                break;
        }
    }
}
