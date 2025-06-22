#ifndef SCREEN2VIEW_HPP
#define SCREEN2VIEW_HPP

#include <gui_generated/screen2_screen/Screen2ViewBase.hpp>
#include <gui/screen2_screen/Screen2Presenter.hpp>

class Screen2View : public Screen2ViewBase
{
public:
    Screen2View();
    virtual ~Screen2View() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
    virtual void tickEvent();

protected:
    Unicode::UnicodeChar numBuffer[4][4][10];
    uint32_t tickCount[4][4] = {{0}};

    void initGame();
    void spawnTile();
    void updateUI();
};

#endif // SCREEN2VIEW_HPP
