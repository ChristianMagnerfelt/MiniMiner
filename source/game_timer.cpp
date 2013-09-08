///	Brief:			This is an implementation of the Game Timer class.
///
///	Description:	The interal format used for the calculations within the class is done in seconds.
///					There is no particular reason for this other than it makes it simple for chaning funtionality, its easier
///					to think about time in seconds rather than millions of microseconds.
///					
///					The functionality of the class depends on the function startTime which calculates everything that 
///					might be of need such as FPS, deltaTime or smooth delta time. The reason for doing the most calculations in frameStart is because
///					you want most of it to be ready and calculated as soon as you using your game code in your main loop.
///
///					A loop using this class might look like this:
///					
///					while(running)
///					{
///						gameTime->startFrame() 
///						// Do game logic stuff
///						// Render Entities and UI
///					}
///
///					-- It doesn't matter where you call the startFrame function but in 
///					order keep stuff structured its recommended to place it at the beginning
///					of your main loop.
///
///	Author:			Christian Magnerfelt
///	Date:			2013.04.21
#include "game_timer.hpp"

#include <functional>

GameTimer::GameTimer() : 
	m_frameStart(),
	m_deltaTime(),
	m_smoothDeltaTime(),
	m_timeAcc(),
	m_targetDeltaTime(seconds_t(1.0f / 60.0f)),
	m_fps(),
	m_frameAcc(),
	m_getTimeFunc(),
	m_stallFunc()
{}
bool GameTimer::init(std::function<seconds_t(void)> getTimeFunc, std::function<void(seconds_t)> stallFunc)
{
	m_getTimeFunc = getTimeFunc;
	m_stallFunc = stallFunc;
	return true;
}
GameTimer::seconds_t GameTimer::getDeltaTime() const
{
	return m_deltaTime;
}
GameTimer::seconds_t GameTimer::getSmoothDeltaTime() const
{
	return m_smoothDeltaTime;
}
std::size_t GameTimer::getFPS() const
{
	return m_fps;
}
///	Brief:	Calculates deltaTime, smoothDeltaTime and accumulates the frame time.
///			When 1 second has passed the FPS is calculated and saved and counters are reset.
///			Before doing any calculations startFrame() calls waitForEndOfFrame in 
///			order to fix the frame time before starting the next.			
void GameTimer::startFrame()
{
	waitUntilEndOfFrame();

	m_deltaTime = calculateDeltaTime();
	m_frameStart = m_getTimeFunc();
	if((m_timeAcc) > seconds_t(1.0f))
	{
		m_fps = m_frameAcc;
		m_timeAcc = seconds_t(0.0f);
		m_frameAcc = 0;
	}
	m_timeAcc += m_deltaTime;
	++m_frameAcc;

	m_smoothDeltaTime = calculateSmoothDeltaTime();
}
GameTimer::seconds_t GameTimer::calculateDeltaTime()
{
	return m_getTimeFunc() - m_frameStart;
}
GameTimer::seconds_t GameTimer::calculateSmoothDeltaTime()
{
	seconds_t accum = seconds_t(0.0f);
	for(unsigned int i = 0; i < NUM_SAVED_DELTA_TIMES - 1; ++i)
	{
		accum += m_savedDeltaTime[i];
		m_savedDeltaTime[i + 1] = m_savedDeltaTime[i];
	}
	accum += m_deltaTime;
	m_savedDeltaTime[0] = m_deltaTime;
	return accum / static_cast<seconds_t>(NUM_SAVED_DELTA_TIMES);
}
void GameTimer::waitUntilEndOfFrame()
{
	auto dt = calculateDeltaTime();
	while(dt + 0.001 < m_targetDeltaTime)
	{
		m_stallFunc(dt);
		dt = calculateDeltaTime();
	}
}