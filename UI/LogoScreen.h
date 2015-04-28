#ifndef LOGOSCREEN_H
#define LOGOSCREEN_H

#include "UI/UIScreen.h"
#include "UI/InputState.h"

class LogoScreen : public UI::UIScreenWithBackground {
public:
    LogoScreen()
        : frames_(0), switched_(false) {}
    bool key(const UI::KeyInput &key) override;
    virtual void update(UI::InputState &input) override;
    virtual void render() override;
    virtual void sendMessage(const char *message, const char *value) override;
    virtual void createViews() override {}

private:
    void next();
    int frames_;
    bool switched_;
};

#endif // LOGOSCREEN_H
