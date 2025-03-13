# pragma once

# ifdef Console_Build_Dll

# define Console_Api __declspec(dllexport)

# else

# define Console_Api __declspec(dllimport)

# endif