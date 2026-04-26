# pragma once
# include <CommonExport.h>
# include <CommonMacro.h>
# include <Logger.h>
# include <Dx11Interface.h>
# include <Types.h>
# include <Keyboard.h>
# define Context_Instance ZDSJ::Context::getInstance()

namespace ZDSJ
{
	class ApplicationWindowInterface;
	class Dx11Interface;
	class Keyboard;
	class Common_Api Context
	{
		friend ApplicationWindowInterface;
		friend Dx11Interface;
	public:
		static Context* getInstance();
		Delete_Copy_And_Move(Context);
		float windowRate() const;
		static Keyboard* getKeyBoard();
	protected:
		/**
		 * @brief 析构函数
		 */
		~Context();
		void registerWindow(ApplicationWindowInterface* _window);
		void registerDx11(Dx11Interface* _ptr);
	private:
		/**
		 * @brief 构造函数
		 */
		Context();
		ReadOnly(Dx11Interface*, dx11, nullptr, Dx11)
		ReadOnly(ApplicationWindowInterface*, window, nullptr, Window)
		ReadWrite(Point, mouse, {}, Mouse)
		ReadWrite(Position, mouse_word, {}, MouseWorld)
		ReadWrite(int, window_x, 200, WindowX)
		ReadWrite(int, window_y, 100, WindowY)
		ReadWrite(int, window_width, 800, WindowWidth)
		ReadWrite(int, window_height, 600, WindowHeight)
		ReadWrite(bool, word_active, true, WordActive)
		ReadWrite(float, delta_time, true, DeltaTime)
		ReadWrite(float, speed, true, Speed)
		ReadOnly(LoggerConfig, log_config, {}, LoggerConfig)

		void readFromConfig();

		void writeToConfig() const;
	};
}
