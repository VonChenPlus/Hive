#ifndef LOGOSCREEN_H
#define LOGOSCREEN_H

#include "UI/UIScreen.h"
#include "UI/InputState.h"

class LogoScreen : public UI::UIScreen {
public:
    LogoScreen()
        : frames_(0), switched_(false) {}
    bool key(const UI::KeyInput &key) override;
    virtual void update(UI::InputState &input) override;
    virtual void render() override;
    virtual void sendMessage(const char *message, const char *value) override;
    virtual void createViews() override {}
    virtual void drawBackground(UI::UIContext &, int alpha = 1.0) override;

private:
    void next();
    int frames_;
    bool switched_;
};

#endif // LOGOSCREEN_H
