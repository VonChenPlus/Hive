#include "ZlibInBuffer.h"

namespace RFB
{
    ZlibInBuffer::ZlibInBuffer(NInBuffer *inBuffer)
        : inBuffer_(inBuffer) {
        zipstream_ = new z_stream;
        zipstream_->zalloc    = Z_NULL;
        zipstream_->zfree     = Z_NULL;
        zipstream_->opaque    = Z_NULL;
        zipstream_->next_in   = Z_NULL;
        zipstream_->avail_in  = 0;
        if (inflateInit(zipstream_) != Z_OK) {
            delete zipstream_;
            throw _NException_Normal("ZlibInStream: inflateInit failed");
        }
    }

    ZlibInBuffer::~ZlibInBuffer() {
        inflateEnd(zipstream_);
        delete zipstream_;
    }

    void ZlibInBuffer::read(Size length, NBYTE *dest, bool wait) {
        checkBuffer(length);
        NBuffer::read(length, dest, wait);
    }

    void ZlibInBuffer::fillBuffer(Size, bool wait) {
        rawBuffer_.clear();
        const Size BLOCKSIZE = 8192;
        inBuffer_->read(BLOCKSIZE, rawBuffer_, wait);
        zipstream_->next_in = (unsigned char*)rawBuffer_.data();
        zipstream_->avail_in = (uInt)rawBuffer_.size();
        Size curLen = size();
        zipstream_->next_out = (unsigned char*)&data_[0] + curLen;
        this->appendBufferSize(BLOCKSIZE * 2);
        zipstream_->avail_out = uInt(size() - curLen);
        int rc = inflate(zipstream_, Z_SYNC_FLUSH);
        if (rc != Z_OK) {
            throw _NException_Normal("ZlibInStream: inflate failed");
        }
    }
}

