#pragma once

#include "stb_image.h"
#include <GL/glew.h>
#include <iostream>
#include <cassert>

constexpr GLint NUMBER_OF_TEXTURES = 1;  // to be generated, that is
constexpr GLint LEVEL_OF_DETAIL = 0;  // base image level; Level n is the nth mipmap reduction image
constexpr GLint TEXTURE_BORDER = 0;  // this value MUST be zero

GLuint load_texture(const char* filepath);