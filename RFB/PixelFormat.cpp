#include "PixelFormat.h"
#include "ProtocolConnection.h"

namespace RFB
{
    PixelFormat::PixelFormat() {
    }

    PixelFormat::~PixelFormat() {

    }

    bool PixelFormat::equal(const PixelFormat& other) const {
        return (bitsPer_ == other.bitsPer_ &&
                depth_ == other.depth_ &&
                (bigEndian_ == other.bigEndian_ || bitsPer_ == 8) &&
                trueColor_ == other.trueColor_ &&
                (!trueColor_ || (redMax_ == other.redMax_ &&
                greenMax_ == other.greenMax_ &&
                blueMax_ == other.blueMax_ &&
                redShift_ == other.redShift_ &&
                greenShift_ == other.greenShift_ &&
                blueShift_ == other.blueShift_)));
    }

    void PixelFormat::read(HInBuffer &buffer) {
        bitsPer_ = buffer.readOne<uint8>();
        depth_ = buffer.readOne<uint8>();
        bigEndian_ = buffer.readOne<uint8>();
        trueColor_ = buffer.readOne<uint8>();
        redMax_ = buffer.readOne<uint16>();
        greenMax_ = buffer.readOne<uint16>();
        blueMax_ = buffer.readOne<uint16>();
        redShift_ = buffer.readOne<uint8>();
        greenShift_ = buffer.readOne<uint8>();
        blueShift_ = buffer.readOne<uint8>();
        buffer.skip(3);
    }
}
