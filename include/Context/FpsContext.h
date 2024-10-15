# pragma once
# include <atomic>
# include <assert.h>

# define ZDSJ_FpsContext_Init_Assert assert(ZDSJ::FpsContext::fps_context != nullptr, "check fpsContext has been inited!")

namespace ZDSJ {

	class FpsContext {
	public:
# define instance fps_context
		static FpsContext* fps_context;
		static void initFpsContext();
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
