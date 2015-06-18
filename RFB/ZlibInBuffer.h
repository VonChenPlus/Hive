#ifndef ZLIBINBUFFER_H
#define ZLIBINBUFFER_H

#include "BASE/NBuffer.h"
#include "EXTERNALS/zlib/zlib.h"

namespace RFB
{
    class ZlibInBuffer final : public NInBuffer
    {
    public:
        ZlibInBuffer(NInBuffer *inBuffer);
        ~ZlibInBuffer();

        void read(Size length, NBYTE *dest, bool wait = true) override;

    private:
        void fillBuffer(Size length, bool wait = true);

    private:
        NInBuffer *inBuffer_;
        NBuffer rawBuffer_;
        z_stream_s* zipstream_;
    };
}

#endif // ZLIBINBUFFER_H
