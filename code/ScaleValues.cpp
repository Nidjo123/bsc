#include <iostream>
#include <cmath>
#include <cstring>
#include <png++/png.hpp>
#include "predefs.hpp"

static char const *help = "Arguments: image scale new_scaled_image\n"
  "Scales all pixels by given scale value\n";

int main(int argc, char *argv[]) {
  if (argc == 4) {
    image img(argv[1]);
    const float scale = atof(argv[2]);
    const int width = img.get_width();
    const int height = img.get_height();

    image out(width, height);
    
    for (int y = 0; y < height; y++) {
      for (int x = 0; x < width; x++) {
	out[y][x] = std::round(img[y][x] * scale);
      }
    }

    out.write(argv[3]);
  } else {
    std::cout << help << std::endl;
  }
  
  return 0;
}
