# pragma once
# include <Windows.h>
# include <CommonMacro.h>
# include <MainWindowUiExport.h>
# include <MyWindowInterface.h>
# include <ApplicationWindowInterface.h>
# include <string>
# include <unordered_map>
# include <Slot.h>

namespace ZDSJ
{
	class MyWindowClass;
	class Main_Window_Ui_Api MyMainWindow: public MyWindowInterface, public ApplicationWindowInterface
	{
	public:
		MyMainWindow(DWORD _ex_style, LPCWSTR _class_name, LPCWSTR _window_name, DWORD _style, int _x, int _y, int _width, int _height,
		             HWND _parent, HMENU _menu, HINSTANCE _instance);
		Slot* connect(const std::string& signal, Slot* slot) override;
		template <class... Args>
		void emit(const std::string& signal, Args&&... args)
		{
			auto pair = this->m_slots.find(signal);
			if(pair != this->m_slots.end())
			{
				for (auto slot : std::get<1>(pair->second))
				{
					slot->emit(std::forward<Args>(args)...);
				}
			}
		}
		LRESULT handelMessage(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) override;
		void addHandelMessage(const char* _id, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)> _handler) override;
		bool getMessage(MSG& _msg) override;
		HWND getHandle() const override;
		~MyMainWindow() override;

		Delete_Copy_And_Move(MyMainWindow);

	protected:
		void registerSignal() override;

	private:
		HWND m_handle = nullptr;
		MyWindowClass* m_windowClass = nullptr;
		std::unordered_map<const char*, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)>> m_messageHandler;
		std::unordered_map<std::string, std::tuple<std::string, std::vector<Slot*>>> m_slots;
	};
}
