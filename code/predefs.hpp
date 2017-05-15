#ifndef PREDEFS_HPP
#define PREDEFS_HPP

#include <png++/png.hpp>

typedef png::image<png::gray_pixel> image;

// window size should be odd
const int MAX_WINDOW_SIZE = 63;

#endif
