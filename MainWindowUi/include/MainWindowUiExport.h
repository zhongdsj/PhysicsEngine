# pragma once

# ifdef Main_Window_Ui_Build_Dll

# define Main_Window_Ui_Api __declspec(dllexport)

# else

# define Main_Window_Ui_Api __declspec(dllimport)

# endif