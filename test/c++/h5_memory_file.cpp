// Copyright (c) 2021-2024 Simons Foundation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0.txt
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
// Authors: Thomas Hahn, Nils Wentzell

#include <gtest/gtest.h>
#include <h5/h5.hpp>

#include <cstddef>
#include <fstream>
#include <string>
#include <vector>

TEST(H5, MemoryFile) {
  // test compatibility of on-disk and in-memory files
  auto vec_int = std::vector{1, 2, 3};
  auto vec_dbl = std::vector{4.0, 5.0, 6.0};
  auto vec_str = std::vector<std::string>{"Hello", "there!"};

  // write data to on-disk file
  auto f_disk = h5::file{"on_disk.h5", 'w'};
  h5::write(f_disk, "vec_int", vec_int);
  h5::write(f_disk, "vec_dbl", vec_dbl);
  h5::write(f_disk, "vec_str", vec_str);

  // write data to memory file
  auto f_mem = h5::file{};
  h5::write(f_mem, "vec_int", vec_int);
  h5::write(f_mem, "vec_dbl", vec_dbl);
  h5::write(f_mem, "vec_str", vec_str);

  // write memory file to disk as standard binary data
  auto buf_mem = f_mem.as_buffer();
  std::ofstream ostrm("in_memory.bin", std::ios::binary);
  ostrm.write(reinterpret_cast<char *>(buf_mem.data()), static_cast<long>(buf_mem.size())); // NOLINT (reinterpret cast is wanted here)

  // read h5 file from disk into raw buffer
  std::vector<std::byte> buf_raw;
  std::ifstream istrm{"on_disk.h5", std::ios::binary | std::ios::ate};
  buf_raw.resize(istrm.tellg(), std::byte{0});
  istrm.seekg(0);
  istrm.read(reinterpret_cast<char *>(buf_raw.data()), static_cast<long>(buf_raw.size())); // NOLINT (reinterpret cast is wanted here)

  // compare various files
  for (auto f : {h5::file{"on_disk.h5", 'r'}, h5::file{buf_raw}, h5::file{buf_mem}}) {
    auto vec_int_read = h5::read<std::vector<int>>(f, "vec_int");
    auto vec_dbl_read = h5::read<std::vector<double>>(f, "vec_dbl");
    auto vec_str_read = h5::read<std::vector<std::string>>(f, "vec_str");

    EXPECT_EQ(vec_int, vec_int_read);
    EXPECT_EQ(vec_dbl, vec_dbl_read);
    EXPECT_EQ(vec_str, vec_str_read);
  }
}
