#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <limits>
#include <png++/png.hpp>
#include "BirchfieldTomasi.hpp"
#include "SemiGlobalMatching.hpp"
#include "predefs.hpp"

static char const *help = "Arguments: left_image right_image "
  "max_disparity P1 P2 out_image\n";

int main(int argc, char *argv[]) {
  if (argc == 7) {
    image left(argv[1]);
    image right(argv[2]);
    const int max_disparity = atoi(argv[3]);

    int  P1 = atoi(argv[4]);
    int  P2 = atoi(argv[5]);

    BirchfieldTomasi corresp(left, right, 1, max_disparity);
    SemiGlobalMatching sgm(&corresp, P1, P2);

    const int height = left.get_height();
    const int width = left.get_width();

    image output(width, height);
   
    std::cout << "Starting calculations!" << std::endl;
    int **disparity_map = disparity_map = sgm.calculateDisparities();

    for (int i = 0; i < height; i++) {
      for (int j = 0; j < width; j++) {
	output[i][j] = disparity_map[i][j];
      }
    }

    std::cout << "Writing sgm matching image " << argv[6] << std::endl;
    output.write(argv[6]);

    std::cout << "Done!" << std::endl;
  } else {
    std::cout << help << std::endl;
  }

  return 0;
}
