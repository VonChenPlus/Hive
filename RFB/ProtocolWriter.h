#ifndef PROTOCOLWRITER_H
#define PROTOCOLWRITER_H

namespace RFB
{
    class ProtocolConnection;

    class ProtocolWriter
    {
    public:
        ProtocolWriter(ProtocolConnection &connection);
        ~ProtocolWriter();

        void initClient(bool shared);

    private:
        ProtocolConnection &connection_;
    };
}

#endif // PROTOCOLWRITER_H
