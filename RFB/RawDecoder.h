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

            for (int y = blocks.minY(); y < blocks.maxY(); y += BLOCKSIZE) {
                for (int x = blocks.minX(); x < blocks.maxX(); x += BLOCKSIZE) {
                    MATH::Recti currBlock(MATH::Vector2i(x,y),
                                          MATH::Vector2i(
                                              std::min(blocks.maxX(), x + BLOCKSIZE)
                                            , std::min(blocks.maxY(), y + BLOCKSIZE)));

                    inBuffer.readAny(currBlock.area() * sizeof(PIXEL), interBuffer);
                    handler.handleImage(currBlock, interBuffer);
                }
            }
        }
    };
}

#endif // RAWDECODER_H
