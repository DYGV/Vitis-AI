/*
 * Copyright 2019 Xilinx Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>

cv::Mat process_result(cv::Mat &image, const vitis::ai::RoadLineResult &result,
                       bool is_jpeg) {
  std::vector<int> color1 = {0, 255, 0, 0, 100, 255};
  std::vector<int> color2 = {0, 0, 255, 0, 100, 255};
  std::vector<int> color3 = {0, 0, 0, 255, 100, 255};

  LOG_IF(INFO, is_jpeg) << "lines.size " << result.lines.size() << " ";
  for (auto &line : result.lines) {
    LOG_IF(INFO, is_jpeg) << "line.points_cluster.size() "
                          << line.points_cluster.size() << " ";
    std::vector<cv::Point> points_poly = line.points_cluster;
    int type = line.type < 5 ? line.type : 5;
    if (type == 2 && points_poly[0].x < image.rows * 0.5) continue;
    cv::polylines(image, points_poly, false,
                  cv::Scalar(color1[type], color2[type], color3[type]), 3,
                  CV_AA, 0);
  }
  return image;
}
