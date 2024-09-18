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

#ifndef _PARTITION_H_
#define _PARTITION_H_

#include <string>
#include <vector>
#include <dirent.h>
#include <unistd.h>

#include "process.hpp"

#define MALI_CLASS_PATH "/sys/class/misc"
#define MALI_DEVICE_PATH "/sys/devices/platform"

using namespace std;

class mali_partition
{
    private:
        string partition_name;
        string status;
        string slices;
        string assigned_aw;
        uint64_t memory_usage; // in kB
        vector<mali_process> processes;

    public:
        // Getter
        string get_partition_name() { return partition_name; };
        string get_status() { return status; };
        string get_slices() { return slices; };
        string get_assigned_aw() { return assigned_aw; };
        uint64_t get_memory_usage() { return memory_usage; };
        vector<mali_process> get_processes() { return processes; };
        // Setter
        void set_status();
        void set_slices();
        int set_slices(string s);
        void set_assigned_aw();
        int set_assigned_aw(string aw);
        void set_memory_usage();
        void set_processes();
        // Constructor / Destructor
        mali_partition(string part);
        ~mali_partition() { processes.clear(); };
        //
        void update();
};

#endif // _PARTITION_H_