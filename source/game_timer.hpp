///	Brief:			The GameTime class provides several useful features when dealing with game time. 
///
///	Description:	Some of the more important features of this class are the ability to calculate FPS, delta time or smooth delta time. 
///					In order to make use of these features the function frameStart() must be called at the beginning of each frame.
///					When starting the frame a function waitUntilEndOfFrame() is called which is used to stall a frame until
///					the desired frame time is achieved, in other words to keep the FPS fixed.
///					The internal clock represents by passing a single function that retrieves the current time stamp to make()
///					The passed function must return time in seconds using float.
///	Author:			Christian Magnerfelt
///	Date			2013.04.21
#ifndef GAME_TIMER_HPP
#define GAME_TIMER_HPP

#include <cstdlib>
#include <functional>

class GameTimer
{
	enum { NUM_SAVED_DELTA_TIMES = 16 };
	public:
		typedef float seconds_t;
		GameTimer::GameTimer();
		bool init(std::function<seconds_t(void)> getTimeFunc, std::function<void(seconds_t)> m_stallFunc);

		seconds_t getDeltaTime() const;
		seconds_t getSmoothDeltaTime() const;
		std::size_t getFPS() const;
		void startFrame();

	private:
		seconds_t m_frameStart;			/// The start of the frame in seconds
		seconds_t m_deltaTime;			/// Delta time of the previous frame
		seconds_t m_smoothDeltaTime;	/// A delta time average over several frames
		seconds_t m_timeAcc;			/// Time accumulated so far since the previous fps calculation
		seconds_t m_targetDeltaTime;	/// The desirable delta time of each frame in order to achieve a fixed frame rate
		std::size_t m_fps;				/// The frames per second
		std::size_t m_frameAcc;			/// Number of frames accumulated during the past second

		seconds_t m_savedDeltaTime [NUM_SAVED_DELTA_TIMES];	/// Time points used for averaging the smooth delta time. NUM_SAVED_DELTA_TIME delta times are saved backwards since the current frame.
		std::function<seconds_t(void)> m_getTimeFunc;
		std::function<void(seconds_t)> m_stallFunc;

		seconds_t calculateDeltaTime();
		seconds_t calculateSmoothDeltaTime();
		void waitUntilEndOfFrame();
};
#endif