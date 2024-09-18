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

#include "process.hpp"
#include "utils.hpp"


/*
 * Get process command from system using PID
 */
void mali_process::set_cmd()
{
    cmd = get_file_content(string("/proc/") + pid + string("/cmdline"));
}

/*
 * Get GPU memory usage from system using PID
 */
void mali_process::set_memory_usage()
{
    string gpu_mem_f = string(MALI_DBG_PATH) + "/" + partition_name + "/gpu_memory";
    string gpu_mempages;
    fstream file_fs;

    file_fs.open(gpu_mem_f, fstream::in);

    if (file_fs.is_open())
    {
        while (getline(file_fs, gpu_mempages))
        {
            if (gpu_mempages.find("pid: "+ pid) != string::npos)
            {
                gpu_mempages.replace(0, gpu_mempages.find(pid)+ pid.length(), "");
                break;
            }
        }

        if (gpu_mempages != "")
            memory_usage = stoi(gpu_mempages) * 4; // in kB
        else
            memory_usage = -1;
    }

    file_fs.close();
}

/*
 * Constructor
 */ 
mali_process::mali_process(string part, string proc_id)
{ 
    partition_name = part;
    pid = proc_id;
    set_cmd();
    set_memory_usage();
}
