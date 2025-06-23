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
    virtual void restart();
    public void playMoveSound(uint32_t freq, uint32_t durationMs);
protected:
    uint32_t tickCount[4][4] = {{0}};
    int score = 0;

    Unicode::UnicodeChar numBuffer[4][4][6];
    Unicode::UnicodeChar scoreBuffer[10];
    Unicode::UnicodeChar highScoreBuffer[10];

    void initGame();
    void spawnTile();
    void updateUI();

    void moveUp();
    void moveDown();
    void moveLeft();
    void moveRight();
    bool isGameOver();
    void saveHighScoreToFlash();
    void loadHighScoreFromFlash();

    void playGameOverSound();
};

#endif // SCREEN2VIEW_HPP
