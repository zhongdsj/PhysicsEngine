# pragma once
# include <string>
# include <unordered_map>
# include <functional>

namespace ZDSJ {
	class Context;
	using HandlerFunc = std::function<void(float _data)>;
	enum Key {
		mouse_wheel = 522,
		nothing = 0,
		mouse_left,
		mouse_righ,
		mouse_middle = 4,
		ctrl = 17,
		space = 32,
		tilde=192,
	};

	class Keyboard {
		friend Context;
	public:
		void mod(int _mod);
		bool execKeyboard(int _key, float _data);
		// -method: registeKeyboard
		// -brief: 注册快捷键
		// -param: int _mod 需要的mod，如ctrl等，不需要时参数为nothing
		// -param: int _key 键位
		// -param: const std::string& _description 快捷键描述
		// -param: void(float _data) _handle_func 快捷键触发回调
		bool registeKeyboard(int _mod, int _key, const std::string& _description, HandlerFunc _handle_func);
	private:
		std::unordered_map<int, std::unordered_map<int, std::pair<std::string, HandlerFunc>>> m_keyboard_handle;
		int m_mod = 0;

		Keyboard(Context* _context);
		std::string outputToCommand() const;
		~Keyboard();
	};

}