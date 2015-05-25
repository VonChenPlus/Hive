#include "LogoScreen.h"
#include "SGF/UI/UIContext.h"
#include "SGF/UI/ScreenManager.h"
#include "MATH/Bounds.h"
using MATH::Bounds;
#include "SGF/GFX/DrawBuffer.h"
using GFX::DrawBuffer;
using GFX::ALIGN_CENTER;
#include "UTILS/COLOR/Color.h"
using UI::InputState;
using UI::KeyInput;
using UI::DEVICE_ID_MOUSE;
using UI::UIContext;
#include "MainScreen.h"

void LogoScreen::next() {
    screenManager()->switchScreen(new MainScreen());
}

void LogoScreen::update(InputState &input_state) {
    UIScreen::update(input_state);
    frames_++;
    if (frames_ > 180 || input_state.pointer_down[0]) {
        next();
    }
}

void LogoScreen::sendMessage(const char *message, const char *value) {
    UNUSED(message);
    UNUSED(value);
}

bool LogoScreen::key(const KeyInput &key) {
    if (key.deviceId != DEVICE_ID_MOUSE) {
        next();
        return true;
    }
    return false;
}

void LogoScreen::render() {
    UIScreen::render();
    UIContext &dc = *screenManager()->getUIContext();

    const Bounds &bounds = dc.getBounds();

    dc.begin();
    float t = (float)frames_ / 60.0f;

    float alpha = t;
    if (t > 1.0f)
        alpha = 1.0f;
    float alphaText = alpha;
    if (t > 2.0f)
        alphaText = 3.0f - t;

    drawBackground(dc, alpha);

    char temp[256];
    snprintf(temp, sizeof(temp), "Created by Feng Chen");
    dc.draw()->drawImage(I_ICON, bounds.centerX() - 80, bounds.centerY() - 30, 1.2f, UTILS::COLOR::ColorAlpha(0xFFFFFFFF, alphaText), ALIGN_CENTER);
    dc.draw()->drawImage(I_LOGO, bounds.centerX() + 20, bounds.centerY() - 10, 1.2f, UTILS::COLOR::ColorAlpha(0xFFFFFFFF, alphaText), ALIGN_CENTER);
    dc.setFontScale(1.0f, 1.0f);
    dc.setFontStyle(dc.theme->uiFont);
    dc.drawText(temp, bounds.centerX(), bounds.centerY() + 40, UTILS::COLOR::ColorAlpha(0xFFFFFFFF, alphaText), ALIGN_CENTER);

    dc.end();
    dc.flush();
}
