#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <limits>
#include <png++/png.hpp>
#include "LocalMatching.hpp"
#include "SSD.hpp"
#include "ZSAD.hpp"
#include "Census.hpp"
#include "BirchfieldTomasi.hpp"
#include "SemiGlobalMatching.hpp"
#include "predefs.hpp"

static char const *help = "Arguments: left_image right_image matching_method"
  "window_size max_disparity output_image P1 P2\n"
  "Available correspondences: SSD, ZSAD, Census, BT\n"
  "Window size must be odd number.\n";

int main(int argc, char *argv[]) {
  if (argc == 9) {
    image left(argv[1]);
    image right(argv[2]);
    std::string corresp_method(argv[3]);
    const int window = atoi(argv[4]);
    const int max_disparity = atoi(argv[5]);

    int  P1 = atoi(argv[7]);
    int  P2 = atoi(argv[8]);

    std::cout << "Left image: '" << argv[1] << "'\nRight image: '" << argv[2] << "'\nWindow size: " << window << "\nMax. disparity: " << max_disparity << std::endl;

    Correspondence *corresp;
    Matching *localMatching;
    Matching *sgm;

    if (corresp_method == "SSD") {
      corresp = new SSD(left, right, window, max_disparity);
    } else if (corresp_method == "ZSAD") {
      corresp = new ZSAD(left, right, window, max_disparity);
    } else if (corresp_method == "Census") {
      corresp = new ZSAD(left, right, window, max_disparity);
    } else if (corresp_method == "BT") {
      corresp = new BirchfieldTomasi(left, right, window, max_disparity);
    } else {
      std::cout << "Unknown correspondence method: " << corresp_method << std::endl;
      return 1;
    }
    
    localMatching = new LocalMatching(corresp);
    sgm = new SemiGlobalMatching(corresp, P1, P2);

    std::cout << "Starting calculations!" << std::endl;
    int **disparity_map = localMatching->calculateDisparities();

    const int width = corresp->getWidth();
    const int height = corresp->getHeight();
    
    // make image
    png::image<png::gray_pixel> output(width, height);

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
	output[i][j] = disparity_map[i][j];
      }
    }

    std::cout << "Writing local matching image " << argv[6] << std::endl;
    output.write("local_" + std::string(argv[6]));

    disparity_map = sgm->calculateDisparities();

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
	output[i][j] = disparity_map[i][j];
      }
    }

    std::cout << "Writing sgm matching image " << argv[6] << std::endl;
    output.write("sgm_" + std::string(argv[6]));

    std::cout << "Done!" << std::endl;

    delete localMatching;
    delete sgm;
    delete corresp;
  } else {
    std::cout << help << std::endl;
  }

  return 0;
}
