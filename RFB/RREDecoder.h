#ifndef RREDECODER_H
#define RREDECODER_H

#include "BASE/Native.h"
#include "MATH/NRect.h"
#include "BASE/NBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"

namespace RFB
{
    class RREDecoder final : public Decoder
    {
    public:
        RREDecoder();
        ~RREDecoder();

        void readRect(const MATH::Rect &block, DataHandler &handle) override;

    private:
        template <typename PIXEL>
        void rreDecode(const MATH::Rect &blocks, NBuffer &inBuffer, RFB::DataHandler &handler) {
            int nSubrects = 0;
            inBuffer.take(sizeof(int), (NBYTE *)&nSubrects);
            PIXEL bg;
            inBuffer.take(sizeof(PIXEL), (NBYTE *)&bg);
            handler.handleFill(blocks, bg);

            for (int index = 0; index < nSubrects; ++index) {
                PIXEL pix;
                MATH::Rect currBlock;
                inBuffer.take(sizeof(PIXEL), (NBYTE *)&pix);
                int xPos, yPos, width, height;
                inBuffer.take(sizeof(uint16), (NBYTE *)&xPos);
                inBuffer.take(sizeof(uint16), (NBYTE *)&yPos);
                inBuffer.take(sizeof(uint16), (NBYTE *)&width);
                inBuffer.take(sizeof(uint16), (NBYTE *)&height);
                currBlock.setRect(xPos, yPos, width, height);
                handler.handleFill(currBlock, pix);
            }
        }
    };
}
#endif // RREDECODER_H
