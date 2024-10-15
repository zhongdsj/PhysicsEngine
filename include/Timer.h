# pragma once
# include <windows.h>
# include <chrono>

namespace ZDSJ {
	class FpsContext;

	class Timer {
	public:
		Timer(uint16_t _fps);
		uint64_t getClockFreq() const;
		void mark();
		void nextFps() const;
		~Timer();
	private:
		LARGE_INTEGER* m_clock_freq = nullptr;

		uint64_t m_last = 0l;
		uint64_t m_interval = 0l;
		uint16_t m_fps = 0l;

		uint64_t now() const;
	};
}