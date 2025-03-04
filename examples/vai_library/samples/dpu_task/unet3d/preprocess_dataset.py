"""
Copyright 2019 Xilinx Inc.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
"""

import os
import argparse
import nibabel
import numpy as np
import torch
from torch.nn.functional import interpolate

EXCLUDED_CASES = []#[23, 68, 125, 133, 15, 37]
MAX_ID = 210
MEAN_VAL = 101.0
STDDEV_VAL = 76.9
MIN_CLIP_VAL = -79.0
MAX_CLIP_VAL = 304.0
TARGET_SPACING = [1.6, 1.2, 1.2]
TARGET_SHAPE = [128, 128, 128]

class Stats:
    def __init__(self):
        self.mean = []
        self.std = []
        self.d = []
        self.h = []
        self.w = []

    def append(self, mean, std, d, h, w):
        self.mean.append(mean)
        self.std.append(std)
        self.d.append(d)
        self.h.append(h)
        self.w.append(w)

class Preprocessor:
    def __init__(self, args):
        self.mean = MEAN_VAL
        self.std = STDDEV_VAL
        self.min_val = MIN_CLIP_VAL
        self.max_val = MAX_CLIP_VAL
        self.results_dir = args.results_dir
        self.data_dir = args.data_dir
        self.target_spacing = TARGET_SPACING
        self.stats = Stats()

    def preprocess_dataset(self):
        os.makedirs(self.results_dir, exist_ok=True)
        print(f"Preprocessing {self.data_dir}")
        for case in sorted([f for f in os.listdir(self.data_dir) if "case" in f]):
            case_id = int(case.split("_")[1])
            if case_id in EXCLUDED_CASES or case_id >= MAX_ID:
                print("Case {}. Skipped.".format(case_id))
                continue
            image, label, image_spacings = self.load_pair(case)
            image1, label = self.preprocess_case(image, label, image_spacings)
            image, label = self.pad_to_min_shape(image1, label)
            self.save(image, label, case)

    def preprocess_case(self, image, label, image_spacings):
        image, label = self.resample3d(image, label, image_spacings)
        image = self.normalize_intensity(image.copy())
        return image, label

    @staticmethod
    def pad_to_min_shape(image, label):
        current_shape = image.shape[1:]
        bounds = [max(0, TARGET_SHAPE[i] - current_shape[i]) for i in range(3)]
        paddings = [(0, 0)]
        paddings.extend([(bounds[i] // 2, bounds[i] - bounds[i] // 2) for i in range(3)])
        return np.pad(image, paddings, mode="edge"), np.pad(label, paddings, mode="edge")

    def load_pair(self, case: str):
        image = nibabel.load(os.path.join(self.data_dir, case, "imaging.nii.gz"))
        label = nibabel.load(os.path.join(self.data_dir, case, "segmentation.nii.gz"))

        image_spacings = image.header["pixdim"][1:4].tolist()
        image, label = image.get_fdata().astype(np.float32), label.get_fdata().astype(np.uint8)
        image, label = np.expand_dims(image, 0), np.expand_dims(label, 0)
        print("image shape: ", image.shape)
        print("label shape: ", label.shape)
        return image, label, image_spacings

    def resample3d(self, image, label, image_spacings):
        if image_spacings != self.target_spacing:
            spc_arr = np.array(image_spacings)
            targ_arr = np.array(self.target_spacing)
            shp_arr = np.array(image.shape[1:])
            new_shape = (spc_arr / targ_arr * shp_arr).astype(int).tolist()

            print("new shape: ", new_shape)
            image = interpolate(torch.from_numpy(np.expand_dims(image, 0)),
                                size=new_shape, mode='trilinear', align_corners=True)
            label = interpolate(torch.from_numpy(np.expand_dims(label, 0)), size=new_shape, mode='nearest')
            image = np.squeeze(image.numpy(), 0)
            label = np.squeeze(label.numpy(), 0)
        return image, label

    def normalize_intensity(self, image: np.array):
        image = np.clip(image, self.min_val, self.max_val)
        image = (image - self.mean) / self.std
        return image

    def save(self, image, label, case: str):
        image = image.astype(np.float32)
        print("result image shape: ", image.shape)
        label = label.astype(np.uint8)
        mean, std = np.round(np.mean(image, (1, 2, 3)), 2), np.round(np.std(image, (1, 2, 3)), 2)
        print(f"Saving {case} shape {image.shape} mean {mean} std {std}")
        self.stats.append(mean, std, image.shape[1], image.shape[2], image.shape[3])
        np.save(os.path.join(self.results_dir, f"{case}_x.npy"), image, allow_pickle=False)
        np.save(os.path.join(self.results_dir, f"{case}_y.npy"), label, allow_pickle=False)

if __name__ == '__main__':

    PARSER = argparse.ArgumentParser()
    PARSER.add_argument('--data_dir', dest='data_dir', required=True)
    PARSER.add_argument('--results_dir', dest='results_dir', required=True)

    args = PARSER.parse_args()
    preprocessor = Preprocessor(args)
    preprocessor.preprocess_dataset()
