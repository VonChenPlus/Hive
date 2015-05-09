#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "UI/UIScreen.h"
#include "UI/PopScreen.h"
#include "UI/CONTROLS/TextEdit.h"

class TextEditPopupScreen : public UI::PopupScreen
{
public:
    TextEditPopupScreen(std::string *value, std::string &placeholder, const std::string &title, int maxLen)
        : PopupScreen(title, "OK", "Cancel"), value_(value), placeholder_(placeholder), maxLen_(maxLen) {}
    virtual void createPopupContents(UI::ViewGroup *parent) override;

    UI::Event OnChange;

private:
    bool showButtons() const override { return false; }
    virtual void onCompleted(UI::DialogResult result) override;
    UI::TextEdit *edit_;
    std::string *value_;
    std::string textEditValue_;
    std::string placeholder_;
    int step_;
    int maxLen_;
};

class MainScreen final : public UI::UIScreenWithBackground
{
public:
    MainScreen();
    ~MainScreen() {}

protected:
    virtual void createViews() override;

private:
    std::string placeholder_;
    std::string text_;
};

#endif // MAINSCREEN_H
