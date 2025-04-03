# pragma once

# ifdef Renderer_Build_Dll

# define Renderer_Api __declspec(dllexport)

# else

# define Renderer_Api __declspec(dllimport)

# endif