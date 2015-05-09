#include "MainScreen.h"

#include "UI/ScreenManager.h"
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

namespace UI
{
    extern void SetFocusedView(View *view, bool force = false);
}

void TextEditPopupScreen::createPopupContents(UI::ViewGroup *parent) {
    textEditValue_ = *value_;
    LinearLayout *lin = parent->add(new LinearLayout(ORIENT_HORIZONTAL, new LinearLayoutParams((fSize) 300, WRAP_CONTENT)));
    edit_ = new TextEdit(textEditValue_, placeholder_, new LinearLayoutParams(1.0f));
    edit_->setMaxLen(maxLen_);
    lin->add(edit_);

    UI::SetFocusedView(edit_);
}

void TextEditPopupScreen::onCompleted(DialogResult result) {
    if (result == DR_OK) {
        *value_ = edit_->getText();
        EventParams e;
        e.v = edit_;
        OnChange.trigger(e);
    }
}

MainScreen::MainScreen() {
    text_ = "192.168.1.1";
}

void MainScreen::createViews() {
    TextEditPopupScreen *popupScreen = new TextEditPopupScreen(&text_, placeholder_, "IP Address", 100);
    screenManager()->push(popupScreen);
}
