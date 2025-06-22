#include <gui/screen2_screen/Screen2View.hpp>     // View class
#include <touchgfx/Unicode.hpp>                   // Unicode::snprintf
#include <cmsis_os.h>                             // osMessageQueue APIs
#include "main.h"
#include <math.h>
#include <string.h>

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

extern osMessageQueueId_t myQueue01Handle;

void Screen2View::tickEvent()
{

	uint8_t res;
	if(osMessageQueueGetCount(myQueue01Handle) > 0)
	{
		osMessageQueueGet(myQueue01Handle, &res, NULL, osWaitForever);
		if(res == 'U')
		{
			tickCount[0][0] ++;
			Unicode::snprintf(numBuffer[0][0], 10, "%d", tickCount[0][0]);
			num00.setWildcard(numBuffer[0][0]);   // Bắt buộc để set nội dung
			num00.invalidate();             // Bắt buộc để hiển thị

		}

		else if(res == 'R')
		{
			tickCount[0][1] ++;
			Unicode::snprintf(numBuffer[0][1], 10, "%d", tickCount[0][1]);
			num01.setWildcard(numBuffer[0][1]);   // Bắt buộc để set nội dung
			num01.invalidate();             // Bắt buộc để hiển thị

		}

		else if(res == 'D')
		{
			tickCount[0][2] ++;
			Unicode::snprintf(numBuffer[0][2], 10, "%d", tickCount[0][2]);
			num02.setWildcard(numBuffer[0][2]);   // Bắt buộc để set nội dung
			num02.invalidate();             // Bắt buộc để hiển thị

		}

		else if(res == 'L')
		{
		tickCount[0][3] ++;
		Unicode::snprintf(numBuffer[0][3], 10, "%d", tickCount[0][3]);
		num03.setWildcard(numBuffer[0][3]);   // Bắt buộc để set nội dung
		num03.invalidate();             // Bắt buộc để hiển thị
		}
	}
}
