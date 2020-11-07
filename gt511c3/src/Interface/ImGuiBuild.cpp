#define IMGUI_IMPL_OPENGL_LOADER_GLAD

#ifdef GT_PLATFORM_WINDOWS
#include "examples/imgui_impl_opengl3.cpp"
#include "examples/imgui_impl_glfw.cpp"
#endif
#ifdef GT_PLATFORM_LINUX
#include "backends/imgui_impl_opengl3.cpp"
#include "backends/imgui_impl_glfw.cpp"
#endif
