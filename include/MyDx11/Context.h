# pragma once
# include <atomic>
namespace ZDSJ {

	class Context {
	public:
		static Context* getInstance();
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
		inline bool animationRun() {
			return this->m_animation_run.load();
		}
		inline void animationRun(bool _run) {
			this->m_animation_run.store(_run);
		}

	private:
		std::atomic<float> m_fps = 0.0f;
		std::atomic<float> m_use_time = 0.0f;
		std::atomic<bool> m_animation_run = true;

		Context() = default;
	protected:
		~Context() = default;
	};

}