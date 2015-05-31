#ifndef RFBDEFINE_H_
#define RFBDEFINE_H_

namespace RFB
{
    enum
    {
        RFBRAW = 0,
        RFBCOPYRECT = 1,
        RFBRRE = 2,
        RFBCORRE = 4,
        RFBHEXTILE = 5,
        RFBZRLE = 16,
        RFBMAX = 0xFF
    };
}

#endif // RFBDEFINE_H_

