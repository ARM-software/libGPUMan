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

#ifndef _PROCESS_H_
#define _PROCESS_H_

#include <string>

#define MALI_DBG_PATH "/sys/kernel/debug"

using namespace std;


class mali_process
{
    private:
        string partition_name;
        string pid;
        string cmd;
        int64_t memory_usage; // in kB

    public:
        // Getter
        string get_pid() { return pid; };
        string get_partition_name() { return partition_name; };
        string get_cmd() { return cmd; };
        int64_t get_memory_usage() { return memory_usage; };
        // Setter - from system config
        void set_cmd(); 
        void set_memory_usage();
        // Constructor / Destructor
        mali_process(string part, string proc_id);
        ~mali_process() {};
};

#endif // _PROCESS_H_