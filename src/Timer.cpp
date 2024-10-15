# include <Timer.h>
# include <MyDx11/Context.h>

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

void ZDSJ::Timer::nextFps() const
{
	const long long target = mulDiv64(this->m_last + this->m_interval, this->m_clock_freq->QuadPart);
	LARGE_INTEGER now;
	QueryPerformanceCounter(&now);
	const long long temp = (target - now.QuadPart) * 100;
	const float use = 1000.0f / (this->m_interval - temp);
	const long long wait = ((temp * 10.0) / this->m_clock_freq->QuadPart);
	ZDSJ::Context::getInstance()->useTime(use);
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
		ZDSJ::Context::getInstance()->fps(this->m_fps);
		return;
	}
	ZDSJ::Context::getInstance()->fps(use * 1000000000);
	return;
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
