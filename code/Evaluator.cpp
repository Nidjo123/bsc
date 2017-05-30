#include "Evaluator.hpp"
#include <cmath>

double Evaluator::error(image &left_truth, image &right_truth, image &test) {
  const int tolerance = 3;

  const int width = test.get_width();
  const int height = test.get_height();

  int unoccluded_pixels = 0;
  int bad_pixels = 0;
  
  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      // unknown disparity
      if (left_truth[y][x] == 0 || right_truth[y][x] == 0) continue;
      
      if (left_truth[y][x] == right_truth[y][x]) {
	unoccluded_pixels++;
	if (std::abs(test[y][x] - left_truth[y][x]) > tolerance) {
	  bad_pixels++;
	}
      }
    }
  }

  //std::cout << "Bad pixels: " << bad_pixels << "\nTotal unoccluded pixels: " << unoccluded_pixels << std::endl;

  return bad_pixels / (double) unoccluded_pixels;
}

int main(int argc, char *argv[]) {
  if (argc == 4) {
    char *left_truth_path = argv[1];
    char *right_truth_path = argv[2];
    char *test_path = argv[3];

    image left_truth(left_truth_path);
    image right_truth(right_truth_path);
    image test(test_path);

    std::cout << Evaluator::error(left_truth, right_truth, test);
  }
  
  return 0;
}
