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
#pragma once

#include <opencv2/core/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <vitis/ai/configurable_dpu_task.hpp>
#include <vitis/ai/movenet.hpp>

using namespace cv;
using namespace std;

namespace vitis {
namespace ai {
class MovenetImp : public vitis::ai::TConfigurableDpuTask<Movenet> {
 public:
  MovenetImp(const std::string& model_name, bool need_preprocess = true);
  virtual ~MovenetImp();

 private:
  virtual MovenetResult run(const cv::Mat& image) override;
  virtual vector<MovenetResult> run(const vector<cv::Mat>& images) override;
};

}  // namespace ai
}  // namespace vitis
