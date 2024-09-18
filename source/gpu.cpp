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

#include "gpu.hpp"
#include "utils.hpp"


/*
 * Sets Mali GPU name
 */
void mali_gpu::set_name()
{
    string gpuinfo_f = find_file(MALI_GPU_PATH, "gpuinfo");

    name = get_file_content(gpuinfo_f);
}

/*
 * Sets Mali DDK version
 */
void mali_gpu::set_ddk_version()
{
    ddk_version = get_file_content(MALI_DDK_VERSION);
}

/*
 * Sets current Mali GPU memory usage from system
 */
void mali_gpu::set_memory_usage()
{
    memory_usage = 0;

    for(mali_partition& i : partitions)
        memory_usage += i.get_memory_usage();
}

/*
 * Sets total system memory in kB from system
 */
void mali_gpu::set_system_memory()
{
    long pages = sysconf(_SC_PHYS_PAGES);
    long page_size = sysconf(_SC_PAGE_SIZE);

    system_memory = pages * page_size / 1024;
}

/*
 * Sets GPU partitions from system configuration
 */
void mali_gpu::set_partitions()
{
    DIR *dir;
    struct dirent *ent;

    // Set number of partitions
    if ((dir = opendir(MALI_CLASS_PATH)) != NULL)
    {
        // loop in all folders in directory
        while ((ent = readdir(dir)) != NULL)
        {
            string d_name = ent->d_name;
            // only count mali* folders in directory
            if (d_name.find("mali") != string::npos)
            {
                partitions.push_back(mali_partition(d_name));
            }
        }

        closedir(dir);
    }
}

/*
 * Constructor
 */
mali_gpu::mali_gpu(bool emit_yaml)
{
    set_name();
    set_ddk_version();
    set_system_memory();
    set_partitions();
    set_memory_usage();
}

/*
 * Update status
 */
void mali_gpu::update()
{
    for(mali_partition& i : partitions)
        i.update();

    set_memory_usage();
}