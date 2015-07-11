#ifndef RAWDECODER_H
#define RAWDECODER_H

#include "BASE/Honey.h"
#include "MATH/Rectangle.h"
#include "BASE/HBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"

namespace RFB
{
    class RawDecoder final : public Decoder
    {
    public:
        static Decoder* Create();
        ~RawDecoder();

        void readRect(const MATH::Recti &block, DataHandler &handle) override;

    private:
        RawDecoder();

        template <typename PIXEL, int BLOCKSIZE = 16>
        void rawDecode(const MATH::Recti &blocks, HInBuffer &inBuffer,
                           HBYTE *interBuffer, RFB::DataHandler &handler) {
            MATH::Recti currBlock;

            for (currBlock.topLeft.y = blocks.topLeft.y; currBlock.topLeft.y < blocks.bottomRight.y; currBlock.topLeft.y += BLOCKSIZE) {
                currBlock.bottomRight.y = std::min(blocks.bottomRight.y, currBlock.topLeft.y + BLOCKSIZE);

                for (currBlock.topLeft.x = blocks.topLeft.x; currBlock.topLeft.x < blocks.bottomRight.x; currBlock.topLeft.x += BLOCKSIZE) {
                    currBlock.bottomRight.x = std::min(blocks.bottomRight.x, currBlock.topLeft.x + BLOCKSIZE);

                    inBuffer.readAny(currBlock.area() * sizeof(PIXEL), interBuffer);
                    handler.handleImage(currBlock, interBuffer);
                }
            }
        }
    };
}

#endif // RAWDECODER_H
