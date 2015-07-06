#ifndef ZRLEDECODER_H
#define ZRLEDECODER_H

#include "BASE/Honey.h"
#include "MATH/HRect.h"
#include "BASE/HBuffer.h"
#include "../RFB/DataHandler.h"
#include "../RFB/Decoder.h"
#include "ZlibInBuffer.h"

namespace RFB
{
    class ZRLEDecoder final : public Decoder
    {
    public:
        static Decoder* Create();
        ~ZRLEDecoder();

        void readRect(const MATH::Rect &block, DataHandler &handle) override;

    private:
        ZRLEDecoder();

        template <typename PIXEL, int BLOCKSIZE = 64>
        void ZRLEDecode(const MATH::Rect &blocks, HInBuffer &inBuffer,
                           HBYTE *interBuffer, RFB::DataHandler &handler) {
            MATH::Rect currBlock;
            int length = 0;
            inBuffer.readOne(&length);
            ZlibInBuffer zlibBuffer(&inBuffer);

            for (currBlock.topLeft.y = blocks.topLeft.y; currBlock.topLeft.y < blocks.bottomRight.y; currBlock.topLeft.y += BLOCKSIZE) {
                currBlock.bottomRight.y = std::min(blocks.bottomRight.y, currBlock.topLeft.y + BLOCKSIZE);

                for (currBlock.topLeft.x = blocks.topLeft.x; currBlock.topLeft.x < blocks.bottomRight.x; currBlock.topLeft.x += BLOCKSIZE) {
                    currBlock.bottomRight.x = std::min(blocks.bottomRight.x, currBlock.topLeft.x + BLOCKSIZE);

                    uint8 mode = 0;
                    zlibBuffer.readOne(&mode);
                    bool rle = !!(mode & 0x80);
                    int palSize = mode & 0x7F;
                    PIXEL palette[128];
                    for (int i = 0; i < palSize; i++) {
                        zlibBuffer.readOne(&palette[i]);
                    }

                    if (palSize == 1) {
                        PIXEL pix = palette[0];
                        handler.handleFill(currBlock, pix);
                        continue;
                    }

                    if (!rle) {
                        if (palSize == 0) {
                            // raw
                            zlibBuffer.read(currBlock.area() * sizeof(PIXEL) / 8, interBuffer);
                        }
                        else {
                            // packed pixels
                            int bppp = ((palSize > 16) ? 8 : ((palSize > 4) ? 4 : ((palSize > 2) ? 2 : 1)));
                            PIXEL *dataPtr = (PIXEL *)interBuffer;

                            for (int height = 0; height < currBlock.height(); ++height) {
                                PIXEL *endOfLine = dataPtr + currBlock.width();
                                uint8 byte = 0;
                                uint8 nbits = 0;
                                while (dataPtr < endOfLine) {
                                    if (nbits == 0) {
                                        zlibBuffer.readOne(&byte);
                                        nbits = 8;
                                    }
                                    nbits -= bppp;
                                    uint8 index = (byte >> nbits) & ((1 << bppp) - 1) & 0x7F;
                                    *dataPtr++ = palette[index];
                                }
                            }
                        }
                    }
                    else {
                        PIXEL *dataPtr = (PIXEL *)interBuffer;
                        PIXEL *endOfBlock = dataPtr + currBlock.area();

                        if (palSize == 0) {
                            // plain RLE
                            while (dataPtr < endOfBlock) {
                                PIXEL pix = 0;
                                zlibBuffer.readOne(&pix);
                                int len = 1;
                                int8 byte;
                                do {
                                    zlibBuffer.readOne(&byte);
                                    len += byte;
                                } while (byte == 255);

                                if (len > endOfBlock - dataPtr) {
                                    throw _HException_Normal("ZRLEDecode Error!");
                                }

                                while (len-- > 0) *dataPtr++ = pix;
                            }
                        }
                        else {
                            // palette RLE
                            while (dataPtr < endOfBlock) {
                                int index = 0;
                                zlibBuffer.readOne(&index);
                                int len = 1;
                                if (index & 0x80) {
                                    int8 byte;
                                    do {
                                        zlibBuffer.readOne(&byte);
                                        len += byte;
                                    } while (byte == 255);

                                    if (len > endOfBlock - dataPtr) {
                                        throw _HException_Normal("ZRLEDecode Error!");
                                    }
                                }

                                index &= 0x7F;

                                PIXEL pix = palette[index];
                                while (len-- > 0) *dataPtr++ = pix;
                            }
                        }
                    }
                    handler.handleImage(currBlock, interBuffer);
                }
            }
        }
    };
}

#endif // ZRLEDECODER_H
