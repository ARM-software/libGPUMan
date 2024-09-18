/*
 * Copyright (c) 2024 Arm Limited.
 *
 * SPDX-License-Identifier: MIT
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#ifndef _GPU_H_
#define _GPU_H_

#include <string>
#include <dirent.h>
#include <unistd.h>
#include <vector>

#include "partition.hpp"
#include "utils.hpp"

#define MALI_DDK_VERSION "/sys/module/mali_kbase/version"
#define MALI_GPU_PATH "/sys/devices/platform"
#define MALI_CLASS_PATH "/sys/class/misc"

using namespace std;

class mali_gpu
{
    private:
        string name;
        string ddk_version;
        uint64_t system_memory; // in kB
        uint64_t memory_usage;  // in kB
        vector<mali_partition> partitions;

    public:
        // Getter
        string get_name() { return name; };
        string get_ddk_version() { return ddk_version; };
        uint64_t get_system_memory() { return system_memory; };
        uint64_t get_memory_usage() { return memory_usage; };
        vector<mali_partition> get_partitions() { return partitions; };
        // Setter - from system config
        void set_name();
        void set_ddk_version();
        void set_system_memory();
        void set_partitions();
        void set_memory_usage();
        // Constructor/Destructor
        mali_gpu( bool emit_yaml=false );
        ~mali_gpu() { partitions.clear(); };
        //
        void update();
};

#endif // _GPU_H_