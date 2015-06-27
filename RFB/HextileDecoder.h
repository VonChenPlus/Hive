#ifndef HEXTILEDECODER_H
#define HEXTILEDECODER_H

#include <algorithm>

#include "BASE/Honey.h"
#include "MATH/NRect.h"
#include "BASE/HBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"

namespace RFB
{
    class HextileDecoder final : public Decoder
    {
    public:
        HextileDecoder();
        ~HextileDecoder();

        void readRect(const MATH::Rect &block, DataHandler &handle) override;

    private:
        enum HEXTILETYPE : int
        {
            HEXTILERAW = 1 << 0,
            HEXTILEBGSPECIFIED = 1 << 1,
            HEXTILEFGSPECIFIED = 1 << 2,
            HEXTILEANYSUBRECTS = 1 << 3,
            HEXTILESUBRECTSCOLOURED = 1 << 4
        };

        template <typename PIXEL, int BLOCKSIZE = 16>
        void hextileDecode(const MATH::Rect &blocks, HInBuffer &inBuffer,
                           HBYTE *interBuffer, RFB::DataHandler &handler) {
            MATH::Rect currBlock;
            PIXEL bg = 0;
            PIXEL fg = 0;

            for (currBlock.topLeft.y = blocks.topLeft.y; currBlock.topLeft.y < blocks.bottomRight.y; currBlock.topLeft.y += BLOCKSIZE) {
                currBlock.bottomRight.y = std::min(blocks.bottomRight.y, currBlock.topLeft.y + BLOCKSIZE);

                for (currBlock.topLeft.x = blocks.topLeft.x; currBlock.topLeft.x < blocks.bottomRight.x; currBlock.topLeft.x += BLOCKSIZE) {
                    currBlock.bottomRight.x = std::min(blocks.bottomRight.x, currBlock.topLeft.x + BLOCKSIZE);

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
