#ifndef QTTEXTDRAWER_H
#define QTTEXTDRAWER_H

#include "SGF/GFX/TextDrawer.h"

class QTTextDrawer final : public GFX::TextDrawer
{
public:
    QTTextDrawer(THIN3D::Thin3DContext *thin3d);
    ~QTTextDrawer();

    GFX::TextDrawerFontContext *setFont(int size) override;
    void measureString(GFX::TextDrawerFontContext * font, const char *str, float *w, float *h) override;
    void drawString(GFX::TextDrawerFontContext * font, const char *str, uint32 color, GFX::TextStringEntry **entry) override;
};

#endif // QTTEXTDRAWER_H
