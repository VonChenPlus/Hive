#include "MainScreen.h"

#include "UI/ScreenManager.h"
using UI::KeyInput;
using UI::UIContext;

bool MainScreen::key(const KeyInput &) {
    return false;
}

void MainScreen::render() {
    UIScreen::render();
    UIContext &dc = *screenManager()->getUIContext();
    
    dc.begin();

    drawBackground(dc);

    dc.end();
}
