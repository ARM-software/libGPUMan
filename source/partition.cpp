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

#include "partition.hpp"
#include "utils.hpp"


/*
 * Set partition status from system
 */
void mali_partition::set_status()
{
    string status_f = string(MALI_CLASS_PATH) + "/" + partition_name + "/device/power/runtime_status";
    status = get_file_content(status_f);
}

/*
 * Set partition slices from system
 */
void mali_partition::set_slices()
{
    string gpuslices_f;
    string partition_id = partition_name;

    slices = "N/A";
    partition_id.replace(0, 4, "");
    gpuslices_f = find_file(MALI_DEVICE_PATH, "partitions") + "/partition" + partition_id + "/active_slices";
    slices = get_file_content(gpuslices_f);
}

/*
 * Set partition slices from argument
 */
int mali_partition::set_slices(string s)
{
    string gpuslices_f;
    string partition_id = partition_name;

    // Argument should be hex value
    if (s.find("0x") != string::npos)
    {
        partition_id.replace(0, 4, "");
        gpuslices_f = find_file(MALI_DEVICE_PATH, "partitions") + "/partition" + partition_id + "/active_slices";
        set_file_content(s, gpuslices_f);
    }
    else
    {
        cout << "Unable to change assigned slices. Have you specified a hex value (e.g. 0xF)?" << endl;

        return 1;
    }

    return 0;
}

/*
 * Set assigned window from system
 */
void mali_partition::set_assigned_aw()
{
    string gpuaw_f;
    string partition_id = partition_name;

    assigned_aw = "N/A";
    partition_id.replace(0, 4, "");
    gpuaw_f = find_file(MALI_DEVICE_PATH, "partitions") + "/partition" + partition_id + "/assigned_access_windows";

    assigned_aw = get_file_content(gpuaw_f);
}

/*
 * Set assigned window from argument
 */
int mali_partition::set_assigned_aw(string aw)
{
    string gpuaw_f;
    string partition_id = partition_name;

    // Argument should be hex value
    if (aw.find("0x") != string::npos)
    {
        partition_id.replace(0, 4, "");
        gpuaw_f = find_file(MALI_DEVICE_PATH, "partitions") + "/partition" + partition_id + "/assigned_access_windows";
        set_file_content(aw, gpuaw_f);
    }
    else
    {
        cout << "Unable to change assigned slices. Have you specified a hex value (e.g. 0xF)?" << endl;

        return 1;
    }

    return 0;
}

/*
 * Set memory usage from system
 */
void mali_partition::set_memory_usage()
{
    string gpu_mem_f = string(MALI_DBG_PATH) + "/" + partition_name + "/gpu_memory";
    string gpu_mempages;
    fstream file_fs;

    file_fs.open(gpu_mem_f, fstream::in);

    if (file_fs.is_open())
    {
        while (getline(file_fs, gpu_mempages))
        {
            if (gpu_mempages.find(partition_name) != string::npos)
            {
                gpu_mempages.replace(0, gpu_mempages.find(partition_name)+ partition_name.length(), "");
                break;
            }
        }

        if (gpu_mempages != "")
            memory_usage = stoi(gpu_mempages) * 4; // in kB
        else
            memory_usage = 0;
    }

    file_fs.close();
}

/*
 * Set running processes from system
 */
void mali_partition::set_processes()
{
    DIR *dir_ctx;
    struct dirent *ent_ctx;
    string path_ctx = string(MALI_DBG_PATH) + "/" + partition_name + "/ctx";

    // get context information if available
    if ((dir_ctx = opendir(path_ctx.c_str())) != NULL)
    {
        while ((ent_ctx = readdir(dir_ctx)) != NULL)
        {
            string pid_thid = ent_ctx->d_name;                   // get pid and thread id
            string pid = pid_thid.substr(0, pid_thid.find("_")); // get pid
            // get rid of ., .. and default
            if (is_number(pid))
            {
                processes.push_back(mali_process(partition_name, pid));
            }
        }

        closedir(dir_ctx);
    }
}

/*
 * Constructor
 */
mali_partition::mali_partition(string part)
{
    partition_name = part; 
    set_status();
    set_slices();
    set_assigned_aw();
    set_memory_usage();
    set_processes();
}

/*
 * Update status
 */
void mali_partition::update()
{
    set_status();
    set_slices();
    set_assigned_aw();
    set_memory_usage();
    processes.clear();
    set_processes();
}