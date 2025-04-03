# pragma once
# include <Windows.h>
# include <CommonMacro.h>
# include <MainWindowUiExport.h>
# include <MyWindowInterface.h>
# include <ApplicationWindowInterface.h>
#include <memory>
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
				std::vector<Slot*>& temp = std::get<1>(pair->second);
				for (auto iterator = temp.begin(); iterator != temp.end(); ) {
					if((*iterator)->useful())
					{
						(*iterator)->emit(std::forward<Args>(args)...);
						++iterator;
					}else
					{
						const Slot* erase = (*iterator);
						delete erase;
						iterator = temp.erase(iterator);
					}
				}
			}
		}
		LRESULT handleMessage(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param) override;
		void addHandleMessage(const char* _id, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)> _handler) override;
		void removeHandleMessage(const char* _id) override;
		bool getMessage(MSG& _msg) override;
		HWND getHandle() const override;
		~MyMainWindow() override;

		Delete_Copy_And_Move(MyMainWindow);

	protected:
		void registerSignal() override;

	private:
		HWND m_handle = nullptr;
		std::shared_ptr<MyWindowClass> m_windowClass;
		std::unordered_map<const char*, std::function<LRESULT(HWND handle, UINT msg, WPARAM w_param, LPARAM l_param)>> m_messageHandler;
		std::unordered_map<std::string, std::tuple<std::string, std::vector<Slot*>>> m_slots;
	};
}
