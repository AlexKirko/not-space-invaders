#pragma once

// GLEW: for loading OpenGL functions newer than OpenGL 1.1
#define GLEW_STATIC
#include <GL/glew.h>
// GLFW: OpenGL framework - cross-platform window
// and access to context
#include <GLFW/glfw3.h>

#include <iostream>

// This debug output funciton is passed to glDebugMessageCallback
// to output debugging information
//
// Can be used only after opening a window and setting the current context.
void APIENTRY opengl_debug_output(GLenum source,
    GLenum type,
    unsigned int id,
    GLenum severity,
    GLsizei length,
    const char* message,
    const void* userParam);
