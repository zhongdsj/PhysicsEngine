# pragma once

# ifdef Common_Build_Dll

# define Common_Api __declspec(dllexport)

# else

# define Common_Api __declspec(dllimport)

# endif