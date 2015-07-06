#include "RawDecoder.h"

#include <memory>

namespace RFB
{
    Decoder* RawDecoder::Create() {
        return new RawDecoder();
    }

    RawDecoder::RawDecoder() {

    }

    RawDecoder::~RawDecoder() {

    }

    void RawDecoder::readRect(const MATH::Rect &block, DataHandler &handle) {
        HBYTE buffer[16 * 16 * 4];
        switch (handle.getPixelSize()) {
            case 8:
                rawDecode<uint8>(block, handle.getInBuffer(), buffer, handle);
                break;
            case 16:
                rawDecode<uint16>(block, handle.getInBuffer(), buffer, handle);
                break;
            case 32:
            default:
                rawDecode<uint32>(block, handle.getInBuffer(), buffer, handle);
                break;
        }
    }
}
