#ifndef LOGOSCREEN_H
#define LOGOSCREEN_H

#include "SMARTGRAPH/UI/UIScreen.h"
#include "SMARTGRAPH/UI/InputState.h"

class LogoScreen final : public UI::UIScreenWithBackground {
public:
    LogoScreen()
        : frames_(0), switched_(false) {}
    ~LogoScreen() {}

protected:
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
