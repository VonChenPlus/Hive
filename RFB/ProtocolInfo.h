#ifndef PROTOCOLINFO_H
#define PROTOCOLINFO_H

#include "BASE/Honey.h"
#include "PixelFormat.h"
#include "MATH/Rectangle.h"

namespace RFB
{
    class ProtocolInfo
    {
    public:
        ProtocolInfo();
        ~ProtocolInfo();

        void setVersion(const std::string &version);
        void setVersion(int major, int minor);

        bool isVersion(int major, int minor);
        bool beforeVersion(int major, int minor);
        bool afterVersion(int major, int minor);

        int majorVersion() { return majorVersion_; }
        int minorVerison() { return minorVersion_; }

        void setDesktopSize(uint16 width, uint16 height);
        MATH::Recti getDesktopSize() { return MATH::Recti(0, 0, width_, height_); }

        void setPixelFormat(const PixelFormat &format) { pixelFormat_ = format; }
        PixelFormat &getPixelFormat() { return pixelFormat_; }

        void setName(const std::string name) { name_ = name; }
        const std::string &getName() { return name_; }

        void supportsLocalCursor(bool support) { supportsLocalCursor_ = support; }
        bool supportsLocalCursor() { return supportsLocalCursor_; }

        void supportsDesktopResize(bool support) { supportsDesktopResize_ = support; }
        bool supportsDesktopResize() { return supportsDesktopResize_; }

    private:
        int majorVersion_;
        int minorVersion_;

        uint16 width_;
        uint16 height_;

        PixelFormat pixelFormat_;

        bool supportsLocalCursor_;
        bool supportsDesktopResize_;

        std::string name_;
    };
}

#endif // PROTOCOLINFO_H
