#ifndef PROTOCOLSECURITY_H
#define PROTOCOLSECURITY_H

namespace RFB
{
    class ProtocolConnection;

    enum SecurityType
    {
        secTypeInvalid = 0,

        secTypeNone = 1,
        secTypeVncAuth = 2,

        secTypeRA2 = 5,
        secTypeRA2ne = 6,

        secTypeSSPI = 7,
        secTypeSSPIne = 8,

        secTypeTight = 16,
        secTypeUltra = 17,
        secTypeTLS = 18
    };

    enum SecurityResult
    {
        secResultUnknown = -1,
        secResultOK = 0,
        secResultFailed = 1,
        secResultTooMany = 2
    };

    class ProtocolSecurity
    {
    public:
        virtual ~ProtocolSecurity() {}

        virtual bool process(ProtocolConnection &) = 0;

        virtual SecurityType getType() = 0;
        virtual const char *description() const = 0;
    };

    class UserPasswdGetter {
      public:
        // getUserPasswd gets the username and password.  This might involve a
        // dialog, getpass(), etc.  The user buffer pointer can be null, in which
        // case no user name will be retrieved.  The caller MUST delete [] the
        // result(s).
        virtual void getUserPasswd(char** userName, char** password)=0;
      };

    class ProtocolSecurityNone final: public ProtocolSecurity
    {
    public:
        bool process(ProtocolConnection &) override;

        SecurityType getType();
        const char *description() const;
    };

    class ProtocolSecurityVncAuth final: public ProtocolSecurity
    {
    public:
        ProtocolSecurityVncAuth(UserPasswdGetter *pwdGetter);

        bool process(ProtocolConnection &conn) override;

        SecurityType getType();
        const char *description() const;

    private:
        UserPasswdGetter *pwdGetter_;
    };
}

#endif // PROTOCOLSECURITY_H
