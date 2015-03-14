#ifndef CALIBRATION_HPP
#define CALIBRATION_HPP

#include "math/matd.h"
#include <vector>
#include <fstream>
#include <iostream>
#include "hsv.hpp"

class calibration_t{
private:
  matd_t *tx_mat;
public:
  calibration_t();
  ~calibration_t();
  void read_tx_mat(ifstream file);
  max_min_hsv get_cyan();
  max_min_hsv get_green();
  max_min_hsv get_red();

  // [x1, x2, y1, y2]
  vector<float> get_mask();

  // [x, y, 1]
  vector<double> translate(vector<double> cam_coords);
};

#endif //CALIBRATION_HPP
