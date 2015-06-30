#ifndef PROTOCOLREADER_H
#define PROTOCOLREADER_H

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
    private:
        ProtocolConnection &connection_;
    };
}

#endif // PROTOCOLREADER_H
