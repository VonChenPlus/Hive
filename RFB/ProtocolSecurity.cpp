#include "ProtocolSecurity.h"

#include <algorithm>
#include <string.h>

#include "ProtocolConnection.h"
#include "EXTERNALS/d3des/d3des.h"

namespace RFB
{
    bool ProtocolSecurityNone::process(ProtocolConnection &) {
        return true;
    }

    SecurityType ProtocolSecurityNone::getType() {
        return secTypeNone;
    }

    const char *ProtocolSecurityNone::description() const {
        return "No Encryption";
    }

    static const int vncAuthChallengeSize = 16;

    ProtocolSecurityVncAuth::ProtocolSecurityVncAuth(UserPasswdGetter *pwdGetter)
        : pwdGetter_(pwdGetter) {
        if (pwdGetter_)
            throw _NException_Normal("Password Getter can not be NULL!");
    }

    bool ProtocolSecurityVncAuth::process(ProtocolConnection &conn) {
        // Read the challenge & obtain the user's password
        uint8 challenge[vncAuthChallengeSize] = {0};
        conn.getInBuffer().readAny(vncAuthChallengeSize, challenge);

        char *passwd = "123";
        pwdGetter_->getUserPasswd(NULLPTR, &passwd);

        if (!passwd)
            throw _NException_Normal("UnHandler Exception!");

        // Calculate the correct response
        uint8 key[8] = {0};
        size_t pwdLen = std::min<Size>(strlen(passwd), 8);
        for (Size index = 0; index < pwdLen; ++index) {
            key[index] = passwd[index];
        }
        deskey(key, EN0);
        for (Size index = 0; index < vncAuthChallengeSize; index += 8)
            des(challenge+index, challenge+index);

        // Return the response to the server
        conn.getOutBuffer().writeAny(vncAuthChallengeSize, challenge);
        conn.getOutBuffer().flushBuffer(vncAuthChallengeSize);
        return true;
    }

    SecurityType ProtocolSecurityVncAuth::getType() {
        return secTypeVncAuth;
    }

    const char *ProtocolSecurityVncAuth::description() const {
        return "Vnc Auth Encryption";
    }
}
