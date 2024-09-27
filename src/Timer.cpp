# include <Timer.h>

inline long long mulDiv64(long long _target, long long _freq)
{
	unsigned __int64 high;
	const unsigned __int64 low = _umul128(_target, _freq, &high);
	unsigned __int64 rem;
	return _udiv128(high, low, 1000000000, &rem);
}

ZDSJ::Timer::Timer(uint16_t _fps) : m_fps(_fps), m_interval((1. / _fps) * 1e9)
{
	timeBeginPeriod(1);
	this->m_clock_freq = new LARGE_INTEGER();
	QueryPerformanceFrequency(this->m_clock_freq);

}

uint64_t ZDSJ::Timer::getClockFreq() const
{
	return this->m_clock_freq->QuadPart;
}

void ZDSJ::Timer::mark()
{
	this->m_last = this->now();

}

double ZDSJ::Timer::nextFps() const
{
	const long long target = mulDiv64(this->m_last + this->m_interval, this->m_clock_freq->QuadPart);
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	const long long wait = (((target - now.QuadPart) * 1000.0) / this->m_clock_freq->QuadPart);
	if (now.QuadPart < target) {

		if (wait > 1) {
			Sleep(wait - 1);
		}
		for (;;) {
			QueryPerformanceCounter(&now);
			if (now.QuadPart >= target) {
				break;
			}

			YieldProcessor();
		}
		return this->m_fps;
	}
	return this->m_fps / (1 - wait * this->m_fps / 1000.0);

}

ZDSJ::Timer::~Timer()
{
	delete this->m_clock_freq;
	timeEndPeriod(1);
}

uint64_t ZDSJ::Timer::now() const
{
	return std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::steady_clock::now().time_since_epoch()).count();

}
