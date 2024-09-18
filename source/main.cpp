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

#include <iostream>
#include <cstring>
#include <bitset>
#include <sstream>

#include "utils.hpp"
#include "process.hpp"
#include "partition.hpp"
#include "gpu.hpp"

using namespace std;

/*
 * Outputs a load bar in the terminal
 */
string load_bar(uint64_t val, uint64_t ref)
{
    int width = 30;
    float pct = (float)val/(float)ref * 100.0f;
    string out = "[";

    for (int i=0; i<width; i++)
    {
        if(i<=pct*width/100)
            out.append("#");
        else
            out.append(" ");
    }
    out.append("] ");
    if(pct < 100/width)
        out.append(" <"+to_string(100/width));
    else
        out.append(to_string(static_cast<int>(pct)));
    out.append("%");

    return out;    
}

/*
 * Convert string representing hex value to list of bit id
 */
string hex_to_id(string val)
{
    string bin, ret="";
    uint64_t n, i=val.length();

    istringstream(val) >> std::hex >> n;
    bin = std::bitset<4>{n}.to_string();

    // Reads indexes in reverse
    for(char& c : bin) 
    {
        if(c == '1')
        {
            ret += to_string(i);
            ret += " ";
        }

        i--;
    }

    return ret;
}

/*
 * Printable gpu class
 */
class printable_mali_gpu : public mali_gpu
{
    public:
        bool display_yaml;
        printable_mali_gpu( bool emit_yaml=false ) { display_yaml = emit_yaml; };
        ~printable_mali_gpu() {};
};

/*
 * Print processes
 */
ostream& operator<<(ostream& os, mali_process& obj) 
{
    os << "      PID " << obj.get_pid() << ":" << endl;
    os << "        Command: " << obj.get_cmd() << endl;
    if(obj.get_memory_usage() >= 0)
        os << "        Memory usage (kB): " << obj.get_memory_usage() << endl;

    return os;
}

/*
 * Print partition
 */
ostream& operator<<(ostream& os, mali_partition& obj) 
{
    vector<mali_process> procs = obj.get_processes();

    os << "  Partition " << obj.get_partition_name() << ":" << endl;
    os << "    Status: " << obj.get_status() << endl;
    if(obj.get_slices() != "N/A")
        os << "    Allocated slice ID(s): " << hex_to_id(obj.get_slices()) << endl;
    if(obj.get_assigned_aw() != "N/A")
        os << "    Assigned access window ID: " << hex_to_id(obj.get_assigned_aw()) << endl;
    os << "    Memory usage (kB): " << obj.get_memory_usage() << endl;
    os << "    Running processes: ";

    if(procs.empty())
        os << "None" << endl;
    else{
        os << endl;
        for(mali_process& i : procs)
            cout << i;
    }

    return os;
}

/*
 * Print gpu
 */
ostream& operator<<(ostream& os, printable_mali_gpu& obj) 
{
    vector<mali_partition> part = obj.get_partitions();

    if(part.empty())
        os << "Could not found any Mali GPU" << endl;
    else
    {
        if(obj.display_yaml)
        {
            os << "---" << endl;
        }
        os << "GPU configuration: " << endl;
        os << "  Name: " << obj.get_name() << endl;
        if(obj.get_ddk_version() != "N/A")
            os << "  DDK version: " << obj.get_ddk_version() << endl;
        os << "  Available partitions: " << obj.get_partitions().size() << endl;
        os << "  GPU memory usage (kB): ";
        if(obj.display_yaml)
            os << obj.get_memory_usage() << endl;
        else
            os << "         " << load_bar(obj.get_memory_usage(), obj.get_system_memory()) << " system memory" << endl;
        if(!obj.display_yaml)
        {
            if(obj.get_partitions().size() > 1)
            {
                for(mali_partition& i : part)
                {
                    os << "    Partition " << i.get_partition_name() << " memory usage: ";
                    os << load_bar(i.get_memory_usage(), obj.get_memory_usage()) << " GPU memory usage" << endl;
                }
            }
        }
        
        os << "  Total system memory (kB): " << obj.get_system_memory() << endl;

        if(!obj.display_yaml)
            os << endl;

        for(mali_partition& i : part)
            cout << i;
    }

    return os;
}


int main(int argc, char *argv[])
{
    bool emit_yaml = false, auto_update = false;
    string slices = "", p_slices = "", aw = "", p_aw = "";
    printable_mali_gpu *device;

    for (int i = 1; i < argc; i++)
    {
        if ((!strcmp(argv[i], "-y")) || (!strcmp(argv[i], "--yaml")))
        {
            emit_yaml = true;
        }
        if ((!strcmp(argv[i], "-u")) || (!strcmp(argv[i], "--update")))
        {
            auto_update = true;
        }
        if ((!strcmp(argv[i], "-s")) || (!strcmp(argv[i], "--slices")))
        {
            i++;
            string tmp = string(argv[i]);
            size_t pos = tmp.find(":");
            p_slices = tmp.substr(0, pos);
            slices = tmp.erase(0, pos + 1);
        }
        if ((!strcmp(argv[i], "-a")) || (!strcmp(argv[i], "--access_window")))
        {
            i++;
            string tmp = string(argv[i]);
            size_t pos = tmp.find(":");
            p_aw = tmp.substr(0, pos);
            aw = tmp.erase(0, pos + 1);
        }
        if ((!strcmp(argv[i], "-h")) || (!strcmp(argv[i], "--help")))
        {
            cout << "Arm Mali GPU monitoring tool" << endl;
            cout << "Usage: ./mali_manager [-h|--help] [-y|--yaml] [-s|--slices PARTITION:SLICES] [-a|--access_window PARTITION:AW]" << endl;
            cout << "   Monitoring mode:"                                                                                            << endl;
            cout << "       -h/--help: print this help and exit"                                                                     << endl;
            cout << "       -y/--yaml: output in YAML format"                                                                        << endl;
            cout << "       -u/--update: automatically update every second"                                                          << endl;
            cout << "   Configuration mode:"                                                                                         << endl;
            cout << "       -s/--slices: assign hex value SLICES to partition PARTITION"                                             << endl;
            cout << "       -a/--access_window: assign hex value AW to partition PARTITION"                                          << endl;

            return EXIT_SUCCESS;
        }
    }

    device = new printable_mali_gpu(emit_yaml);

    if(slices != "" || aw != "")
    {
        if(slices != "")
        {
            if(device->get_partitions()[std::stoi(p_slices)].set_slices(slices))
            {
                cout << "Failed to assign slice ID(s) [" << hex_to_id(slices) << "] to partition " << p_slices << endl;
                return EXIT_FAILURE;
            }
            else
                cout << "Successfully assigned slice ID(s) [" << hex_to_id(slices) << "] to partition " << p_slices << endl;
        }
        if(aw != "")
        {
            if(device->get_partitions()[std::stoi(p_aw)].set_assigned_aw(aw))
            {
                cout << "Failed to assign access window ID [" << hex_to_id(aw) << "] to partition " << p_aw << endl;
                return EXIT_FAILURE;
            }
            else
                cout << "Successfully assigned access window ID [" << hex_to_id(aw) << "] to partition " << p_aw << endl;
        }
        return EXIT_SUCCESS;
    }

    if(auto_update)
    {
        while(1)
        {
            cout << "\033[2J";    // clear the screen
            cout << "\033[1;1H";  // move cursor home
            cout << *device;
            sleep(1);
            device->update();
        }
    }
    else
        cout << *device;

    delete device;

    return EXIT_SUCCESS;
}