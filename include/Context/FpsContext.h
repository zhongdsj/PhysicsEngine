# pragma once
# include <atomic>

namespace ZDSJ {

	class FpsContext {
	public:
		static FpsContext* getInstance();
		inline float fps() const {
			return this->m_fps.load();
		}
		inline void fps(float _fps) {
			this->m_fps.store(_fps);
		}
		inline float useTime() const {
			return this->m_use_time.load();
		}
		inline void useTime(float _use_time) {
			this->m_use_time.store(_use_time);
		}
	private:
		std::atomic<float> m_fps = 0.0f;
		std::atomic<float> m_use_time = 0.0f;

		FpsContext() = default;
	protected:
		~FpsContext() = default;
	};

}
