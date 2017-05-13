#include <iostream>
#include <cstring>
#include <cmath>
#include <limits>
#include <png++/png.hpp>
#include "LocalMatching.hpp"
#include "SSD.hpp"
#include "ZSAD.hpp"
#include "predefs.hpp"

int main(int argc, char *argv[]) {
  if (argc == 6) {
    image left(argv[1]);
    image right(argv[2]);
    const int window = atoi(argv[3]);
    const int max_disparity = atoi(argv[4]);

    std::cout << "Left image: '" << argv[1] << "'\nRight image: '" << argv[2] << "'\nWindow size: " << window << "\nMax. disparity: " << max_disparity << std::endl;

    ZSAD corresp(left, right, window, max_disparity);

    LocalMatching localMatching(&corresp);

    std::cout << "Starting calculations!" << std::endl;
    int **disparity_map = localMatching.calculateDisparities();

    const int width = corresp.getWidth();
    const int height = corresp.getHeight();
    
    // make image
    png::image<png::gray_pixel> output(width, height);

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
	output[i][j] = std::round(disparity_map[i][j]);
      }
    }

    std::cout << "Writing image " << argv[5] << std::endl;
    output.write(argv[5]);

    std::cout << "Done!" << std::endl;
  }

  return 0;
}
