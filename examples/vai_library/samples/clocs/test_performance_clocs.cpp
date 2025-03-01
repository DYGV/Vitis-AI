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
#include <sys/stat.h>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <vitis/ai/benchmark.hpp>
#include <vitis/ai/clocs.hpp>
using namespace vitis::ai;
using namespace vitis::ai::clocs;
using std::vector;

static void read_points_file(const std::string& points_file_name,
                             std::vector<float>& points) {
  struct stat file_stat;
  if (stat(points_file_name.c_str(), &file_stat) != 0) {
    std::cerr << "file:" << points_file_name << " state error!" << std::endl;
    exit(-1);
  }
  auto file_size = file_stat.st_size;
  points.resize(file_size / 4);
  CHECK(std::ifstream(points_file_name)
            .read(reinterpret_cast<char*>(points.data()), file_size)
            .good());
}

namespace vitis {
namespace ai {

static void read_clocs_info(std::string& input_file_name,
                            clocs::ClocsInfo& clocs_info) {
  std::string input_path;
  auto pos = input_file_name.find_last_of('/');
  std::string idx_str = input_file_name;
  if (pos != std::string::npos) {
    input_path = input_file_name.substr(0, pos + 1);
    idx_str = input_file_name.substr(pos + 1);
  }
  idx_str = idx_str.substr(0, idx_str.find_last_of('.'));
  // LOG(INFO) << "input_path:" << input_path;
  // LOG(INFO) << "idx_str:" << idx_str;
  auto points_file_name = input_path + idx_str + ".bin";
  auto image_file_name = input_path + idx_str + ".png";

  // 1. read float bin
  // input_file : 000001.bin
  auto input_file = std::string(points_file_name);
  std::vector<float> input;
  read_points_file(input_file, input);

  // int last_dim = 4;
  // int points_num = input.size() / last_dim;

  // std::cout << "input shape: " << last_dim << " * " << points_num
  //          << std::endl;

  // 2. read image
  // image: 000001.png
  auto input_image = cv::imread(image_file_name);
  // std::cout << "input_image rows:" << input_image.rows
  //          << ", cols:" << input_image.cols << std::endl;

  std::vector<float> p2;
  std::vector<float> trv2c;
  std::vector<float> rect;
  read_points_file(input_path + idx_str + "_P2.bin", p2);
  read_points_file(input_path + idx_str + "_Trv2c.bin", trv2c);
  read_points_file(input_path + idx_str + "_rect.bin", rect);
  clocs_info.calib_P2.assign(p2.begin(), p2.end());
  clocs_info.calib_Trv2c.assign(trv2c.begin(), trv2c.end());
  clocs_info.calib_rect.assign(rect.begin(), rect.end());
  clocs_info.image = input_image;
  clocs_info.points = input;
}

class ClocsPerf {
 public:
  ClocsPerf(std::string input_n, std::string yolo, std::string pointpillars_0,
            std::string pointpillars_1, std::string fusion)
      : input_name(input_n),
        kernel_yolo(yolo),
        kernel_pp_0(pointpillars_0),
        kernel_pp_1(pointpillars_1),
        kernel_fusion(fusion),
        det(Clocs::create(yolo, pointpillars_0, pointpillars_1, fusion)) {
    batch_size = det->get_input_batch();
    all_arrays.resize(batch_size);
    for (size_t i = 0; i < batch_size; i++) {
      read_clocs_info(input_name, all_arrays[i]);
    }
  }
  int getInputWidth() { return det->getInputWidth(); }
  int getInputHeight() { return det->getInputHeight(); }
  size_t get_input_batch() { return (size_t)det->get_input_batch(); }
  std::vector<ClocsResult> run(const std::vector<cv::Mat>& image) {
    auto res = det->run(all_arrays);
    return res;
  }

 private:
  std::string input_name;
  std::string kernel_yolo;
  std::string kernel_pp_0;
  std::string kernel_pp_1;
  std::string kernel_fusion;
  size_t batch_size;
  std::unique_ptr<Clocs> det;
  std::vector<vitis::ai::clocs::ClocsInfo> all_arrays;
};

}  // namespace ai
}  // namespace vitis

int main(int argc, char* argv[]) {
  std::string input = argv[5];
  std::string model_1 = argv[1];
  std::string model_2 = argv[2];
  std::string model_3 = argv[3];
  std::string model_4 = argv[4];
  std::string input_file_name = "./000001.txt";
  if (input.find_last_of('/') != std::string::npos) {
    input_file_name =
        input.substr(0, input.find_last_of('/') + 1) + input_file_name;
  }

  return vitis::ai::main_for_performance(
      argc, argv, [input_file_name, model_1, model_2, model_3, model_4] {
        {
          return std::unique_ptr<vitis::ai::ClocsPerf>(new vitis::ai::ClocsPerf(
              input_file_name, model_1, model_2, model_3, model_4));
        }
      });
}
