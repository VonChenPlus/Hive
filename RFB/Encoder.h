#ifndef ENCODER_H
#define ENCODER_H

#include "BASE/Honey.h"

namespace RFB
{
    enum Encoding : uint32
    {
        encodingRaw = 0,
        encodingCopyRect = 1,
        encodingRRE = 2,
        endcodingCoRRE = 4,
        encodingHextile = 5,
        encodingZRLE = 16,
        encodingMax = 255,

        pseudoEncodingCursor = 0xFFFFFF11,
        pseudoEncodingDesktopSize = 0xFFFFFF21
    };
}

#endif // ENCODER_H

