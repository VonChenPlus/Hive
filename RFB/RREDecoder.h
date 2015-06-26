#ifndef RREDECODER_H
#define RREDECODER_H

#include "BASE/Honey.h"
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
        void rreDecode(const MATH::Rect &blocks, NInBuffer &inBuffer, RFB::DataHandler &handler) {
            int nSubrects = 0;
            inBuffer.readAny(sizeof(int), &nSubrects);
            PIXEL bg;
            inBuffer.readAny(sizeof(PIXEL), &bg);
            handler.handleFill(blocks, bg);

            for (int index = 0; index < nSubrects; ++index) {
                PIXEL pix;
                MATH::Rect currBlock;
                inBuffer.readAny(sizeof(PIXEL), &pix);
                int xPos, yPos, width, height;
                inBuffer.readAny(sizeof(uint16), &xPos);
                inBuffer.readAny(sizeof(uint16), &yPos);
                inBuffer.readAny(sizeof(uint16), &width);
                inBuffer.readAny(sizeof(uint16), &height);
                currBlock.setRect(xPos, yPos, width, height);
                handler.handleFill(currBlock, pix);
            }
        }
    };
}
#endif // RREDECODER_H
