#ifndef ZLIBINBUFFER_H
#define ZLIBINBUFFER_H

#include "BASE/NBuffer.h"
#include "EXTERNALS/zlib/zlib.h"

namespace RFB
{
    class ZlibInBuffer final : protected NBuffer
    {
    public:
        ZlibInBuffer(NBuffer *inBuffer);
        ~ZlibInBuffer();

        void take(Size length, NBYTE *dest, bool wait = true) override;

    private:
        void fillBuffer(Size length, bool wait = true);

    private:
        NBuffer *inBuffer_;
        NBuffer rawBuffer_;
        z_stream_s* zipstream_;
    };
}

#endif // ZLIBINBUFFER_H
