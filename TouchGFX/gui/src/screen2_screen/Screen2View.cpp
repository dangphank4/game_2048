#include <gui/screen2_screen/Screen2View.hpp>     // View class
#include <touchgfx/Unicode.hpp>                   // Unicode::snprintf
#include <cmsis_os.h>                             // osMessageQueue APIs
#include "main.h"
#include <math.h>
#include <string.h>

extern osMessageQueueId_t myQueue01Handle;
extern int highScore;
Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
    initGame();

}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}


void Screen2View::initGame()
{
    memset(tickCount, 0, sizeof(tickCount));
    score = 0;

    spawnTile();
    spawnTile();
    updateUI();
}
//

void Screen2View::spawnTile()
{
    int emptyCount = 0;
    int emptyPos[16][2];

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (tickCount[i][j] == 0)
                emptyPos[emptyCount][0] = i, emptyPos[emptyCount++][1] = j;

    if (emptyCount > 0)
    {
        int idx = HAL_GetTick() % emptyCount;
        int i = emptyPos[idx][0];
        int j = emptyPos[idx][1];
        tickCount[i][j] = (HAL_GetTick() % 10 == 0) ? 4 : 2;
    }
}

void Screen2View::updateUI()
{
    Unicode::snprintf(scoreBuffer, 10, "%d", score);
    scoreText.setWildcard(scoreBuffer);
    scoreText.invalidate();

    Unicode::snprintf(highScoreBuffer, 10, "%d", highScore);
    highscoreText.setWildcard(highScoreBuffer);
    highscoreText.invalidate();
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tickCount[i][j] == 0)
                Unicode::snprintf(numBuffer[i][j], 10, "");
            else
                Unicode::snprintf(numBuffer[i][j], 10, "%d", tickCount[i][j]);

            // map 4x4 tile: tile00, tile01, ..., tile33
            switch (i * 4 + j)
            {
            case 0: num00.setWildcard(numBuffer[i][j]); num00.invalidate(); break;
            case 1: num01.setWildcard(numBuffer[i][j]); num01.invalidate(); break;
            case 2: num02.setWildcard(numBuffer[i][j]); num02.invalidate(); break;
            case 3: num03.setWildcard(numBuffer[i][j]); num03.invalidate(); break;
            case 4: num10.setWildcard(numBuffer[i][j]); num10.invalidate(); break;
            case 5: num11.setWildcard(numBuffer[i][j]); num11.invalidate(); break;
            case 6: num12.setWildcard(numBuffer[i][j]); num12.invalidate(); break;
            case 7: num13.setWildcard(numBuffer[i][j]); num13.invalidate(); break;
            case 8: num20.setWildcard(numBuffer[i][j]); num20.invalidate(); break;
            case 9: num21.setWildcard(numBuffer[i][j]); num21.invalidate(); break;
            case 10: num22.setWildcard(numBuffer[i][j]); num22.invalidate(); break;
            case 11: num23.setWildcard(numBuffer[i][j]); num23.invalidate(); break;
            case 12: num30.setWildcard(numBuffer[i][j]); num30.invalidate(); break;
            case 13: num31.setWildcard(numBuffer[i][j]); num31.invalidate(); break;
            case 14: num32.setWildcard(numBuffer[i][j]); num32.invalidate(); break;
            case 15: num33.setWildcard(numBuffer[i][j]); num33.invalidate(); break;
            }
        }
    }

}



void Screen2View::moveUp() {
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            for (int k = x + 1; k < 4; k++) {
                if (tickCount[k][y] > 0) {
                    if (tickCount[x][y] == 0) {
                        tickCount[x][y] = tickCount[k][y];
                        tickCount[k][y] = 0;
                        x--; // kiểm lại ô hiện tại
                    } else if (tickCount[x][y] == tickCount[k][y]) {
                        tickCount[x][y] *= 2;
                        tickCount[k][y] = 0;
                        score += tickCount[x][y];
                        if (score > highScore)
                        	        highScore = score;
                    }
                    break;
                }
            }
        }
    }
}

void Screen2View::moveDown() {
    for (int y = 0; y < 4; y++) {
        for (int x = 3; x > 0; x--) {
            for (int k = x - 1; k >= 0; k--) {
                if (tickCount[k][y] > 0) {
                    if (tickCount[x][y] == 0) {
                        tickCount[x][y] = tickCount[k][y];
                        tickCount[k][y] = 0;
                        x++;
                    } else if (tickCount[x][y] == tickCount[k][y]) {
                        tickCount[x][y] *= 2;
                        tickCount[k][y] = 0;
                        score += tickCount[x][y];
                        if (score > highScore)
                        	        highScore = score;
                    }
                    break;
                }
            }
        }
    }
}

void Screen2View:: moveLeft(){
	for(int x = 0; x < 4; x++){
		for(int y = 0; y < 3; y++){
			for(int k = y + 1; k < 4; k++){
				if(tickCount[x][k] > 0){
					if(tickCount[x][y] == 0){
						tickCount[x][y] = tickCount[x][k];
						tickCount[x][k] = 0;
						y--;
					}else if(tickCount[x][y] == tickCount[x][k]){
						tickCount[x][y] *= 2;
						tickCount[x][k] = 0;
						score += tickCount[x][y];
						if (score > highScore)
							        highScore = score;
					}
					break;
				}
			}
		}
	}
}
void Screen2View::moveRight() {
    for (int x = 0; x < 4; x++) {
        for (int y = 3; y > 0; y--) {
            for (int k = y - 1; k >= 0; k--) {
                if (tickCount[x][k] > 0) {
                    if (tickCount[x][y] == 0) {
                        tickCount[x][y] = tickCount[x][k];
                        tickCount[x][k] = 0;
                        y++; // kiểm lại ô hiện tại sau khi đẩy
                    } else if (tickCount[x][y] == tickCount[x][k]) {
                        tickCount[x][y] *= 2;
                        tickCount[x][k] = 0;
                        score += tickCount[x][y];
                        if (score > highScore)
                        	        highScore = score;
                    }
                    break;
                }
            }
        }
    }
}


void Screen2View::restart(){
	 if (score > highScore)
	        highScore = score;

	 initGame();
}

void Screen2View::tickEvent()
{
    uint8_t res;
    if (osMessageQueueGetCount(myQueue01Handle) > 0)
    {
        osMessageQueueGet(myQueue01Handle, &res, NULL, osWaitForever);

        switch (res)
        {
        case 'U':
            moveUp();
            spawnTile();
            updateUI();
            break;
        case 'D':
            moveDown();
            spawnTile();
            updateUI();
            break;
        case 'L':
            moveLeft();
            spawnTile();
            updateUI();
            break;
        case 'R':
            moveRight();
            spawnTile();
            updateUI();
            break;
        }
    }
}
