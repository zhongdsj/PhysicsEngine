# pragma once
# include <string>
# include <sstream> 
# include <shared_mutex>
# include <functional>
# include <vector>


namespace ZDSJ {
	class Context;

	template <class T>
	class RingBuffer {
		friend Context;
	public: 
		
		//static RingBuffer<T>* getInstance() {
		//	static ZDSJ::RingBuffer<T> instance(32, [](std::string& _value) {  });
		//	return &instance;
		//}

		void clear() {
			while (this->m_begin != this->m_end) {
				this->m_destory_callback(this->m_buffer[this->m_begin]);
				this->m_begin += 1;
				this->m_begin %= this->m_max_size;
			}
		}

		void enablePush(bool _enable) {
			this->m_mutex.lock_shared();
			this->m_enable_push = _enable;
			this->m_mutex.unlock_shared();
		}

		void push(const T& _val) {
			this->m_mutex.lock();
			if (this->m_begin == ((this->m_end + 1) % this->m_max_size)) {
				// 队列已满
				this->m_destory_callback(this->m_buffer[this->m_begin]);
				this->m_begin += 1;
				this->m_begin %= this->m_max_size;
			}
			this->m_buffer[this->m_end] = _val;
			this->m_end += 1;
			this->m_end %= this->m_max_size;
			this->m_mutex.unlock();
		}

		void push(T& _val) {
			this->m_mutex.lock();
			if (!this->m_enable_push) {
				this->m_destory_callback(_val);
				this->m_mutex.unlock();
				return;
			}
			if (this->m_begin == ((this->m_end + 1) % this->m_max_size)) {
				// 队列已满
				this->m_destory_callback(this->m_buffer[this->m_begin]);
				this->m_begin += 1;
				this->m_begin %= this->m_max_size;
			}
			this->m_buffer[this->m_end] = _val;
			this->m_end += 1;
			this->m_end %= this->m_max_size;
			this->m_mutex.unlock();
		}

		void forEeach(std::function<void(const T&)> _for_each_callback) {
			this->m_mutex.lock_shared();
			size_t i = this->m_begin;
			while (i != this->m_end) {
				_for_each_callback(this->m_buffer[i]);
				i += 1;
				i %= this->m_max_size;
			}
			this->m_mutex.unlock_shared();
		}

		template <class ... Args>
		char* format(Args&... _args) {
			std::ostringstream oss;
			this->format(oss, _args...);
			char* s = new char[oss.str().size()];
			memcpy_s(s, oss.str().size(), oss.str().data(), oss.str().size());
			return s;
		}

		template <class T, class... Args>
		void format(std::ostringstream& _oss, T& _format, Args&... _args) {
			this->format(_oss, _format, 0, _args...);
		}

		void format(std::ostringstream& _oss, const std::string& _format, std::size_t _offset)
		{
			_oss << _format.substr(_offset, _format.size() - _offset);
			_oss << '\0';
		}

		template <class T, class... Args>
		void format(
			std::ostringstream& _oss, const std::string& _format, std::size_t _offset, T& _first, Args&... _args)
		{
			std::size_t off = _format.find("{}", _offset);
			if (off == std::string::npos) {
				_oss << _format.substr(_offset, _format.size() - _offset);
				return;
			}
			_oss << _format.substr(_offset, off - _offset) << _first;
			this->format(_oss, _format, off + 2, _args...);
		}

	private:

		T* m_buffer = nullptr;
		bool m_enable_push = true;
		size_t m_begin = 0;
		size_t m_end = 0;
		size_t m_max_size = 0;
		std::function<void(T&)> m_destory_callback;
		std::shared_mutex m_mutex;

		RingBuffer(size_t _max_size, std::function<void(T&)> _destory_callback = [](T& _value) {}) : m_max_size(_max_size + 1), m_destory_callback(_destory_callback) {
			this->m_buffer = new T[_max_size + 1];
		}

		~RingBuffer() {
			while (this->m_begin != this->m_end) {
				this->m_destory_callback(this->m_buffer[this->m_begin]);
				this->m_begin += 1;
				this->m_begin %= this->m_max_size;
			}
		}
	};
}