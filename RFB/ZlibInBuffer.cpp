#include "ZlibInBuffer.h"

namespace RFB
{
    ZlibInBuffer::ZlibInBuffer(HInBuffer *inBuffer)
        : inBuffer_(inBuffer) {
        zipstream_ = new z_stream;
        zipstream_->zalloc    = Z_NULL;
        zipstream_->zfree     = Z_NULL;
        zipstream_->opaque    = Z_NULL;
        zipstream_->next_in   = Z_NULL;
        zipstream_->avail_in  = 0;
        if (inflateInit(zipstream_) != Z_OK) {
            delete zipstream_;
            throw _HException_Normal("ZlibInStream: inflateInit failed");
        }
    }

    ZlibInBuffer::~ZlibInBuffer() {
        inflateEnd(zipstream_);
        delete zipstream_;
    }

    void ZlibInBuffer::read(uint64 length, HBYTE *dest, bool wait) {
        checkBuffer(length);
        HBuffer::read(length, dest, wait);
    }

    void ZlibInBuffer::fillBuffer(uint64, bool wait) {
        rawBuffer_.clear();
        const uint64 BLOCKSIZE = 8192;
        inBuffer_->readAny(BLOCKSIZE, rawBuffer_, wait);
        zipstream_->next_in = (unsigned char*)rawBuffer_.data();
        zipstream_->avail_in = (uInt)rawBuffer_.size();
        uint64 curLen = size();
        zipstream_->next_out = (unsigned char*)&data_[0] + curLen;
        this->appendBufferSize(BLOCKSIZE * 2);
        zipstream_->avail_out = uInt(size() - curLen);
        int rc = inflate(zipstream_, Z_SYNC_FLUSH);
        if (rc != Z_OK) {
            throw _HException_Normal("ZlibInStream: inflate failed");
        }
    }
}

