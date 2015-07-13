#ifndef PROTOCOLREADER_H
#define PROTOCOLREADER_H

#include "BASE/Honey.h"
#include "MATH/Vector.h"
#include "MATH/Rectangle.h"
#include "DataHandler.h"
#include "Decoder.h"

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
        void readFramebufferUpdate();
        void readSetColorMapEntries();
        void readBell();
        void readServerCutText();
        void readCursor(uint16 width, uint16 height, const MATH::Vector2f &hostpot);
        void readRect(const MATH::Recti rect, Encoding encoding);
        void readCopyRect(const MATH::Recti rect);

    private:
        ProtocolConnection &connection_;
        uint16 updateRectsLeft_;

        Decoder *decoder_[encodingMax + 1];
    };
}

#endif // PROTOCOLREADER_H
