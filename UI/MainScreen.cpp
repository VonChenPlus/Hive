#include "MainScreen.h"

#include "GRAPH/UI/ScreenManager.h"
using UI::TextEdit;
using UI::KeyInput;
using UI::UIContext;
using UI::AnchorLayout;
using UI::LinearLayoutParams;
using UI::LinearLayout;
using UI::LayoutParams;
using UI::FILL_PARENT;
using UI::ORIENT_HORIZONTAL;
using UI::WRAP_CONTENT;
using UI::DialogResult;
using UI::DR_OK;
using UI::EventParams;
using UI::KEY_DOWN;

namespace UI
{
    extern void SetFocusedView(View *view, bool force = false);
    extern bool IsEscapeKeyCode(int keyCode);
}

void TextEditPopupScreen::createPopupContents(UI::ViewGroup *parent) {
    textEditValue_ = *value_;
    LinearLayout *lin = parent->add(new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams(300.0, WRAP_CONTENT)));
    edit_ = new TextEdit(textEditValue_, placeholder_, new LinearLayoutParams(1.0f));
    edit_->setMaxLen(maxLen_);
    lin->add(edit_);

    UI::SetFocusedView(edit_);
}

bool TextEditPopupScreen::key(const KeyInput &key) {
    if ((key.flags & KEY_DOWN) && UI::IsEscapeKeyCode(key.keyCode))
        return false;
    return PopupScreen::key(key);
}

bool TextEditPopupScreen::onCompleted(DialogResult result) {
    if (result == DR_OK) {
        *value_ = edit_->getText();
        EventParams e;
        e.v = edit_;
        OnChange.trigger(e);
        return true;
    }
    return false;
}

MainScreen::MainScreen() {
    text_ = "192.168.1.1";
}

bool MainScreen::key(const UI::KeyInput &) {
    return false;
}

void MainScreen::createViews() {
    TextEditPopupScreen *popupScreen = new TextEditPopupScreen(&text_, placeholder_, "IP Address", 100);
    popupScreen->OnChange.handle(this, &MainScreen::HandleChange);
    screenManager()->push(popupScreen);
}

UI::EventReturn MainScreen::HandleChange(UI::EventParams &) {
    return UI::EVENT_DONE;
}
