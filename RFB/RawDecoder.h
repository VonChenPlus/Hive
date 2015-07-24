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

            for (currBlock.origin.y = blocks.origin.y; currBlock.origin.y < blocks.bottom(); currBlock.origin.y += BLOCKSIZE) {
                currBlock.bottom(std::min(blocks.bottom(), currBlock.origin.y + BLOCKSIZE));

                for (currBlock.origin.x = blocks.origin.x; currBlock.origin.x < blocks.right(); currBlock.origin.x += BLOCKSIZE) {
                    currBlock.right(std::min(blocks.right(), currBlock.origin.x + BLOCKSIZE));

                    inBuffer.readAny(currBlock.area() * sizeof(PIXEL), interBuffer);
                    handler.handleImage(currBlock, interBuffer);
                }
            }
        }
    };
}

#endif // RAWDECODER_H
