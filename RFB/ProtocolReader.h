#ifndef PROTOCOLREADER_H
#define PROTOCOLREADER_H

#include "BASE/Honey.h"
#include "MATH/Point.h"
#include "MATH/HRect.h"
#include "Encoder.h"

namespace RFB
{
    class ProtocolConnection;

    enum MessageForRead
    {
        FramebufferUpdate = 0,
        SetColorMapEntries = 1,
        Bell = 2,
        ServerCutText = 3
    };

    class ProtocolReader final
    {
    public:
        ProtocolReader(ProtocolConnection &connection);
        ~ProtocolReader();

        void readServerInitInfo();
        void readMessage();

    private:
        void readCursor(uint16 width, uint16 height, const MATH::Point &hostpot);
        void readRect(const MATH::Rect rect, Encoding encoding);
        void readCopyRect(const MATH::Rect rect);

    private:
        ProtocolConnection &connection_;
        int updateRectsLeft_;
    };
}

#endif // PROTOCOLREADER_H
