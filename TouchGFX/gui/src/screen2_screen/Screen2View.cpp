#include <gui/screen2_screen/Screen2View.hpp>
#include <touchgfx/Unicode.hpp>
#include <cmsis_os.h>
#include "main.h"
#include <math.h>
#include <string.h>

extern osMessageQueueId_t myQueue01Handle;
extern int highScore;

#define FLASH_SECTOR_SAVE      FLASH_SECTOR_6
#define FLASH_SECTOR_ADDRESS   0x08040000  // Bắt đầu sector 6


Screen2View::Screen2View()
{
}

void Screen2View::saveHighScoreToFlash()
{
    HAL_FLASH_Unlock();

    // Xóa sector trước khi ghi
    FLASH_EraseInitTypeDef erase;
    uint32_t sectorError;
    erase.TypeErase    = FLASH_TYPEERASE_SECTORS;
    erase.VoltageRange = FLASH_VOLTAGE_RANGE_3;  // 2.7V - 3.6V
    erase.Sector       = FLASH_SECTOR_SAVE;
    erase.NbSectors    = 1;

    if (HAL_FLASHEx_Erase(&erase, &sectorError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return;
    }

    HAL_FLASH_Program(FLASH_TYPEPROGRAM_WORD, FLASH_SECTOR_ADDRESS, highScore);

    HAL_FLASH_Lock();
}

void Screen2View::loadHighScoreFromFlash()
{
    uint32_t value = *(uint32_t*)FLASH_SECTOR_ADDRESS;

    // Nếu dữ liệu hợp lệ (ví dụ không phải 0xFFFFFFFF)
    if (value != 0xFFFFFFFF)
    {
        highScore = value;
    }
    else
    {
        highScore = 0;
    }
}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
    loadHighScoreFromFlash();
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

    gameOverText.setVisible(false); // Ẩn thông báo thua khi bắt đầu
    gameOverText.invalidate();

    spawnTile();
    HAL_Delay(8);
    spawnTile();
    updateUI();
}

void Screen2View::spawnTile()
{
    int emptyCount = 0;
    int emptyPos[16][2];

    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (tickCount[i][j] == 0)
            {
                emptyPos[emptyCount][0] = i;
                emptyPos[emptyCount][1] = j;
                emptyCount++;
            }
        }
    }

    if (emptyCount > 0)
    {
        // Gọi HAL_GetTick() một lần duy nhất
        uint32_t tick = HAL_GetTick() ;

        // Sử dụng tick để sinh số "ngẫu nhiên"
        int idx = tick % emptyCount;
        int i = emptyPos[idx][0];
        int j = emptyPos[idx][1];

        tick = tick ^ (i * 31 + j * 17);

        // Dùng tick tiếp tục để quyết định giá trị 2 hoặc 4
        tickCount[i][j] = ((tick / 10) % 10 == 0) ? 4 : 2;
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

void Screen2View::moveUp()
{
    for (int y = 0; y < 4; y++) {
        for (int x = 0; x < 3; x++) {
            for (int k = x + 1; k < 4; k++) {
                if (tickCount[k][y] > 0) {
                    if (tickCount[x][y] == 0) {
                        tickCount[x][y] = tickCount[k][y];
                        tickCount[k][y] = 0;
                        x--;
                    } else if (tickCount[x][y] == tickCount[k][y]) {
                        tickCount[x][y] *= 2;
                        tickCount[k][y] = 0;
                        score += tickCount[x][y];
                        if (score > highScore) highScore = score;
                    }
                    break;
                }
            }
        }
    }
}

void Screen2View::moveDown()
{
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
                        if (score > highScore) highScore = score;
                    }
                    break;
                }
            }
        }
    }
}

void Screen2View::moveLeft()
{
    for (int x = 0; x < 4; x++) {
        for (int y = 0; y < 3; y++) {
            for (int k = y + 1; k < 4; k++) {
                if (tickCount[x][k] > 0) {
                    if (tickCount[x][y] == 0) {
                        tickCount[x][y] = tickCount[x][k];
                        tickCount[x][k] = 0;
                        y--;
                    } else if (tickCount[x][y] == tickCount[x][k]) {
                        tickCount[x][y] *= 2;
                        tickCount[x][k] = 0;
                        score += tickCount[x][y];
                        if (score > highScore) highScore = score;
                    }
                    break;
                }
            }
        }
    }
}

void Screen2View::moveRight()
{
    for (int x = 0; x < 4; x++) {
        for (int y = 3; y > 0; y--) {
            for (int k = y - 1; k >= 0; k--) {
                if (tickCount[x][k] > 0) {
                    if (tickCount[x][y] == 0) {
                        tickCount[x][y] = tickCount[x][k];
                        tickCount[x][k] = 0;
                        y++;
                    } else if (tickCount[x][y] == tickCount[x][k]) {
                        tickCount[x][y] *= 2;
                        tickCount[x][k] = 0;
                        score += tickCount[x][y];
                        if (score > highScore) highScore = score;
                    }
                    break;
                }
            }
        }
    }
}

void Screen2View::restart()
{
    if (score > highScore)
        highScore = score;

    saveHighScoreToFlash();

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
        case 'U': moveUp(); break;
        case 'D': moveDown(); break;
        case 'L': moveLeft(); break;
        case 'R': moveRight(); break;
        }

        spawnTile();
        updateUI();

        // Kiểm tra thua
        if (isGameOver()) {
            gameOverText.setVisible(true);
            gameOverText.invalidate();
        }
    }
}

bool Screen2View::isGameOver()
{
    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
            if (tickCount[i][j] == 0)
                return false;

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 4; j++)
        {
            if (i < 3 && tickCount[i][j] == tickCount[i + 1][j]) return false;
            if (j < 3 && tickCount[i][j] == tickCount[i][j + 1]) return false;
        }
    saveHighScoreToFlash();
    return true;
}
