#ifndef ZLIBINBUFFER_H
#define ZLIBINBUFFER_H

#include "BASE/HBuffer.h"
#include "EXTERNALS/zlib/zlib.h"

namespace RFB
{
    class ZlibInBuffer final : public HInBuffer
    {
    public:
        ZlibInBuffer(HInBuffer *inBuffer);
        ~ZlibInBuffer();

        void read(uint64 length, HBYTE *dest, bool wait = true);

    private:
        void fillBuffer(uint64 length, bool wait = true);

    private:
        HInBuffer *inBuffer_;
        HBuffer rawBuffer_;
        z_stream_s* zipstream_;
    };
}

#endif // ZLIBINBUFFER_H
