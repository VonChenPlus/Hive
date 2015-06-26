#include "QTTextDrawer.h"

#include <QtGui/QImage>
#include <QtGui/QPainter>
#include <QtGui/QFontMetrics>
#include <QtOpenGL/QGLWidget>

using GFX::TextDrawer;
using GFX::TextDrawerFontContext;
using GFX::TextStringEntry;
#include "GRAPH/THIN3D/Thin3D.h"
using THIN3D::Thin3DContext;

QTTextDrawer::QTTextDrawer(Thin3DContext *thin3d)
    : TextDrawer(thin3d) {
}

QTTextDrawer::~QTTextDrawer() {

}

TextDrawerFontContext *QTTextDrawer::setFont(int size) {
    QFont* font = new QFont();
    font->setPixelSize(size + 6);
    return (TextDrawerFontContext *)font;
}

void QTTextDrawer::measureString(TextDrawerFontContext * font, const char *str, float *w, float *h) {
    QFont* qfont = (QFont *)font;
    QFontMetrics fm(*qfont);
    QSize size = fm.size(0, QString::fromUtf8(str));
    *w = (float)size.width();
    *h = (float)size.height();
}

void QTTextDrawer::drawString(TextDrawerFontContext * font, const char *str, uint32 color, TextStringEntry **entry) {
    QFont *qfont = (QFont *)font;
    QFontMetrics fm(*qfont);
    QSize size = fm.size(0, QString::fromUtf8(str));
    QImage image((size.width() + 3) & ~ 3, (size.height() + 3) & ~ 3, QImage::Format_ARGB32_Premultiplied);
    if (image.isNull()) {
        return;
    }
    image.fill(0);

    QPainter painter;
    painter.begin(&image);
    painter.setFont(*qfont);
    painter.setPen(color);
    painter.drawText(image.rect(), Qt::AlignTop | Qt::AlignLeft, QString::fromUtf8(str).replace("&&", "&"));
    painter.end();

    (*entry) = new TextStringEntry();
    (*entry)->bmWidth = (*entry)->width = image.width();
    (*entry)->bmHeight = (*entry)->height = image.height();
    uint16 *bitmapData = new uint16[(*entry)->bmWidth * (*entry)->bmHeight];
    for (int x = 0; x < (*entry)->bmWidth; x++) {
        for (int y = 0; y < (*entry)->bmHeight; y++) {
            bitmapData[(*entry)->bmWidth * y + x] = 0xfff0 | image.pixel(x, y) >> 28;
        }
    }
    (*entry)->texture = createTexture(bitmapData, (*entry)->bmWidth, (*entry)->bmHeight);

    delete [] bitmapData;
}
