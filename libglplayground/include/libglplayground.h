#pragma once
// precompiled header
#include "libglplayground/libglppch.h"

// core libglplayground headers
#include "libglplayground/ref.h"
#include "libglplayground/window.h"
#include "libglplayground/renderer.h"
#include "libglplayground/entity.h"
#include "libglplayground/scene.h"
#include "libglplayground/application.h"

// OpenGL object implementations
#include "libglplayground/vertex_array_object.h"
#include "libglplayground/vertex_buffer_object.h"
#include "libglplayground/element_buffer_object.h"
#include "libglplayground/shader.h"
#include "libglplayground/texture.h"

// class for easily reading and creating shaders
#include "libglplayground/shader_factory.h"

// every built-in component; feel free to write more!
#include "libglplayground/components.h"

// basic main implementation
#include "libglplayground/main.h"