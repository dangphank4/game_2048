#include <gui/screen2_screen/Screen2View.hpp>     // View class
#include <touchgfx/Unicode.hpp>                   // Unicode::snprintf
#include <cmsis_os.h>                             // osMessageQueue APIs
#include "main.h"

Screen2View::Screen2View()
{

}

void Screen2View::setupScreen()
{
    Screen2ViewBase::setupScreen();
    setText();
}

void Screen2View::tearDownScreen()
{
    Screen2ViewBase::tearDownScreen();
}

extern osMessageQueueId_t myQueue01Handle;
void Screen2View::setText()
{
    Unicode::snprintf(numBuffer, 10, "%d", 10);
    num00.setWildcard(numBuffer);   // Bắt buộc để set nội dung
    num00.invalidate();             // Bắt buộc để hiển thị
}

void Screen2View::tickEvent()
{

	uint8_t res;
	if(osMessageQueueGetCount(myQueue01Handle) > 0)

	{
		osMessageQueueGet(myQueue01Handle, &res, NULL, osWaitForever);
		if(res == 'X')
		{
			tickCount ++;
			Unicode::snprintf(numBuffer, 10, "%d", tickCount);
			num00.setWildcard(numBuffer);   // Bắt buộc để set nội dung
			num00.invalidate();             // Bắt buộc để hiển thị

		}
	}
}
