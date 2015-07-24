#ifndef HEXTILEDECODER_H
#define HEXTILEDECODER_H

#include <algorithm>

#include "BASE/Honey.h"
#include "MATH/Rectangle.h"
#include "BASE/HBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"

namespace RFB
{
    class HextileDecoder final : public Decoder
    {
    public:
        static Decoder* Create();
        ~HextileDecoder();

        void readRect(const MATH::Recti &block, DataHandler &handle) override;

    private:
        HextileDecoder();

        enum HEXTILETYPE : int
        {
            HEXTILERAW = 1 << 0,
            HEXTILEBGSPECIFIED = 1 << 1,
            HEXTILEFGSPECIFIED = 1 << 2,
            HEXTILEANYSUBRECTS = 1 << 3,
            HEXTILESUBRECTSCOLOURED = 1 << 4
        };

        template <typename PIXEL, int BLOCKSIZE = 16>
        void hextileDecode(const MATH::Recti &blocks, HInBuffer &inBuffer,
                           HBYTE *interBuffer, RFB::DataHandler &handler) {
            MATH::Recti currBlock;
            PIXEL bg = 0;
            PIXEL fg = 0;

            for (currBlock.origin.y = blocks.origin.y; currBlock.origin.y < blocks.bottom(); currBlock.origin.y += BLOCKSIZE) {
                currBlock.bottom(std::min(blocks.bottom(), currBlock.origin.y + BLOCKSIZE));

                for (currBlock.origin.x = blocks.origin.x; currBlock.origin.x < blocks.right(); currBlock.origin.x += BLOCKSIZE) {
                    currBlock.right(std::min(blocks.right(), currBlock.origin.x + BLOCKSIZE));

                    int tileType = 0;
                    inBuffer.readOne(&tileType);
                    if (tileType & HEXTILERAW) {
                        inBuffer.readAny(currBlock.area() * sizeof(PIXEL), interBuffer);
                        handler.handleImage(currBlock, interBuffer);
                        continue;
                    }

                    if (tileType & HEXTILEBGSPECIFIED)
                        inBuffer.readOne(&bg);

                    int area = currBlock.area();
                    PIXEL *dataPtr = (PIXEL *)interBuffer;
                    while (area-- > 0) *dataPtr++ = bg;

                    if (tileType & HEXTILEFGSPECIFIED)
                        inBuffer.readOne(&fg);

                    if (tileType & HEXTILEANYSUBRECTS) {
                        int nSubrects = 0;
                        inBuffer.readOne(&nSubrects);

                        for (int i = 0; i < nSubrects; i++) {
                            if (tileType & HEXTILESUBRECTSCOLOURED)
                                inBuffer.readOne(&fg);

                            int xy, wh;
                            inBuffer.readOne(&xy);
                            inBuffer.readOne(&wh);
                            int x = ((xy >> 4) & 0xF);
                            int y = (xy & 0xF);
                            int w = ((wh >> 4) & 0xF) + 1;
                            int h = (wh & 0xF) + 1;
                            PIXEL *dataPtr = (PIXEL *)interBuffer + y * currBlock.width() + x;
                            int rowAdd = currBlock.width() - w;
                            while (h-- > 0) {
                                int len = w;
                                while (len-- > 0) *dataPtr++ = fg;
                                dataPtr += rowAdd;
                            }
                        }
                    }

                    handler.handleImage(currBlock, interBuffer);
                }
            }
        }
    };
}

#endif // HEXTILEDECODER_H
