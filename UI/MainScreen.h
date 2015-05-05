#ifndef MAINSCREEN_H
#define MAINSCREEN_H

#include "UI/UIScreen.h"

class MainScreen final : public UI::UIScreenWithBackground
{
public:
    MainScreen() {}
    ~MainScreen() {}

protected:
    bool key(const UI::KeyInput &key) override;
    virtual void render() override;
    virtual void createViews() override {}

private:

};

#endif // MAINSCREEN_H
