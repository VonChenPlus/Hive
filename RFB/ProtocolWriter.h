#ifndef PROTOCOLWRITER_H
#define PROTOCOLWRITER_H

#include "BASE/Honey.h"
#include "DataHandler.h"

namespace RFB
{
    class ProtocolConnection;
    class PixelFormat;

    enum MessageForWrite
    {
        SetPixelFormat = 0,
        FixColorMapEntries = 1,
        SetEncodings = 2,
        FramebufferUpdateRequest = 3,
        KeyEvent = 4,
        PointerEvent = 5,
        ClientCutText = 6
    };

    class ProtocolWriter final
    {
    public:
        ProtocolWriter(ProtocolConnection &connection);
        ~ProtocolWriter();

        void writeInitClient(bool shared);
        void writeSetPixelFormat(PixelFormat &foramt);
        void writeSetEncodings(Encoding preferredEncoding, bool useCopyRect);
        void writeFramebufferUpdateRequest(const MATH::Recti& rect, bool incremental);
        void writeKeyEvent(uint32 key, bool down);
        void writeMouseEvent(const MATH::Vector2i &pos, uint8 buttonMask);
        void writeClientCutText(const std::string &cutText);

    private:
        void writeSetEncodings(uint16 nEncodings, Encoding *encodings);

    private:
        ProtocolConnection &connection_;
    };
}

#endif // PROTOCOLWRITER_H
