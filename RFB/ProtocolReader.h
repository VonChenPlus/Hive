#ifndef PROTOCOLREADER_H
#define PROTOCOLREADER_H

namespace RFB
{
    class ProtocolConnection;

    class ProtocolReader
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
