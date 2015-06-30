#ifndef PROTOCOLWRITER_H
#define PROTOCOLWRITER_H

#include "BASE/Honey.h"

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

        void initClient(bool shared);
        void setPixelFormat(PixelFormat &foramt);
        void setEncodings(uint16 nEncodings, uint32 *encodings);

    private:
        ProtocolConnection &connection_;
    };
}

#endif // PROTOCOLWRITER_H
