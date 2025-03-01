// Copyright 2011 Emilie Gillet.
//
// Author: Emilie Gillet (emilie.o.gillet@gmail.com)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
// -----------------------------------------------------------------------------
//
// Global clock.

#include "gridseq.h"
#include "gridres.h"
#include <per/rng.h>

namespace grids {

static float interpf(float v1, float v2, float amt) {
    return v1*amt + v2*(1.0f-amt);
};

static const uint8_t* drum_map[kGridSize][kGridSize] = {
  { node_10, node_8, node_0, node_9, node_11 },
  { node_15, node_7, node_13, node_12, node_6 },
  { node_18, node_14, node_4, node_5, node_3 },
  { node_23, node_16, node_21, node_1, node_2 },
  { node_24, node_19, node_17, node_20, node_22 },
};

float PatternGenerator::ReadDrumMap() {
    auto parse_data = [](uint8_t value) {
        return static_cast<float>(value) / static_cast<float>(0xFF);
    };
 
    uint8_t i = static_cast<uint8_t>(fminf(x, 1.0f) * static_cast<float>(kGridSize));
    uint8_t j = static_cast<uint8_t>(fminf(y, 1.0f) * static_cast<float>(kGridSize));
    const uint8_t* a_map = drum_map[i][j];
    const uint8_t* b_map = drum_map[i + 1][j];
    const uint8_t* c_map = drum_map[i][j + 1];
    const uint8_t* d_map = drum_map[i + 1][j + 1];
    uint8_t offset = (instrument_ * kStepsPerPattern) + step_;
    offset = fminf(offset, kStepsPerPattern * kNumParts);
    float a = parse_data(a_map[offset]);
    float b = parse_data(b_map[offset]);
    float c = parse_data(c_map[offset]);
    float d = parse_data(d_map[offset]);

    return interpf(interpf(a, b, x), interpf(c, d, x), y);
}

void PatternGenerator::Tick() {
    if (step_ == 0) {
        part_perturbation_ = daisy::Random::GetFloat(0.0f, chaos);
    }
  
    level_ = fminf(ReadDrumMap() + part_perturbation_, 1.0f);
    step_ = (step_ + 1) % kStepsPerPattern;
}

bool PatternGenerator::Triggered() {
    return (level_ > 1.0f - fill);
}

void EuclidianGenerator::Tick() {
    step_ = (step_ + 1) % length_;
}

bool EuclidianGenerator::Triggered() {
    size_t offset = 0;
    offset += length_ * kStepsPerPattern;
    offset += fill * kStepsPerPattern;

    return lut_res_euclidean[offset] & (1L << static_cast<uint32_t>(step_));
}


}  // namespace grids

