#ifndef RAWDECODER_H
#define RAWDECODER_H

#include "BASE/Native.h"
#include "MATH/NRect.h"
#include "BASE/NBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"

namespace RFB
{
    class RawDecoder final : public Decoder
    {
    public:
        RawDecoder();
        ~RawDecoder();

        void readRect(const MATH::Rect &block, DataHandler &handle) override;

    private:
        template <typename PIXEL, int BLOCKSIZE = 16>
        void rawDecode(const MATH::Rect &blocks, NBuffer &inBuffer,
                           NBYTE *interBuffer, RFB::DataHandler &handler) {
            MATH::Rect currBlock;

            for (currBlock.topLeft.y = blocks.topLeft.y; currBlock.topLeft.y < blocks.bottomRight.y; currBlock.topLeft.y += BLOCKSIZE) {

                currBlock.bottomRight.y = std::min(blocks.bottomRight.y, currBlock.topLeft.y + BLOCKSIZE);

                for (currBlock.topLeft.x = blocks.topLeft.x; currBlock.topLeft.x < blocks.bottomRight.x; currBlock.topLeft.x += BLOCKSIZE) {

                  currBlock.bottomRight.x = std::min(blocks.bottomRight.x, currBlock.topLeft.x + BLOCKSIZE);

                  inBuffer.take(currBlock.area() * sizeof(PIXEL), interBuffer);
                  handler.handle(currBlock, interBuffer);
                }
            }
        }
    };
}

#endif // RAWDECODER_H
