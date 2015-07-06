#ifndef RREDECODER_H
#define RREDECODER_H

#include "BASE/Honey.h"
#include "MATH/HRect.h"
#include "BASE/HBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"

namespace RFB
{
    class RREDecoder final : public Decoder
    {
    public:
        static Decoder* Create();
        ~RREDecoder();

        void readRect(const MATH::Rect &block, DataHandler &handle) override;

    private:
        RREDecoder();

        template <typename PIXEL>
        void rreDecode(const MATH::Rect &blocks, HInBuffer &inBuffer, RFB::DataHandler &handler) {
            int nSubrects = 0;
            inBuffer.readOne(&nSubrects);
            PIXEL bg;
            inBuffer.readOne(&bg);
            handler.handleFill(blocks, bg);

            for (int index = 0; index < nSubrects; ++index) {
                PIXEL pix;
                MATH::Rect currBlock;
                inBuffer.readOne(&pix);
                uint16 xPos, yPos, width, height;
                inBuffer.readOne(&xPos);
                inBuffer.readOne(&yPos);
                inBuffer.readOne(&width);
                inBuffer.readOne(&height);
                currBlock.setRect(xPos, yPos, width, height);
                handler.handleFill(currBlock, pix);
            }
        }
    };
}
#endif // RREDECODER_H
