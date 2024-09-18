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

#include "utils.hpp"


/*
 * Returns True if the string s is a number
 */
bool is_number(const string &s)
{
    string::const_iterator it = s.begin();

    while (it != s.end() && isdigit(*it))
        ++it;

    return !s.empty() && it == s.end();
}

/*
 * Returns the string content of file fp
 * fp should be an absolute path to the file
 */
string get_file_content(string fp)
{
    string c;
    fstream file_fs;

    file_fs.open(fp, fstream::in);

    if (!file_fs.is_open())
        c = "N/A";
    else
    {
        getline(file_fs, c);
        file_fs.close();
    }

    file_fs.close();

    return c;
}

/*
 * Writes the string content s to file fp
 * fp should be an absolute path to the file
 */
void set_file_content(string s, string fp)
{
    fstream file_fs;

    file_fs.open(fp, fstream::out);

    if (!file_fs.is_open())
        cout << "Failed to open " << fp << endl;
    else
    {
        file_fs << s;
        file_fs.close();
    }

    file_fs.close();
}

/*
 * Checks if path is a directory
 */
bool is_directory(const string path) {
    struct stat statbuf;
    // Shall not be symbolic link with lstat instead of stat
    if (lstat(path.c_str(), &statbuf) != 0)
        return false;

    return S_ISDIR(statbuf.st_mode) != 0;
}

/*
 * Checks if path is a file
 */
bool is_file(const string path) {
    struct stat statbuf;
    // Shall not be symbolic link with lstat instead of stat
    if (lstat(path.c_str(), &statbuf) != 0)
        return false;
    
    return S_ISREG(statbuf.st_mode) != 0;
}

/*
 * Returns path to file/folder name matching f in path p
 */
string find_file(string p, string f)
{
    DIR *dir;
    struct dirent *ent;
    string fp = "";

    if ((dir = opendir(p.c_str())) != NULL)
    {
        // Loop in all folders in directory
        while ((ent = readdir(dir)) != NULL)
        {
            string d_name = ent->d_name;

            // Don't search in . and ..
            if (d_name != "." && d_name != "..")
            {
                string tmp = p + "/" + d_name;

                // If file name matches f
                if (d_name.find(f) != string::npos)
                {
                    fp = tmp;

                    break;
                }
                else
                {
                    if(is_directory(tmp))
                    {
                        fp = find_file(tmp, f);
                        
                        if (fp != "")
                            break;
                    }
                }
            }
        }
        closedir(dir);
    }

    return fp;
}
