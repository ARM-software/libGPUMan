#
# Copyright (c) 2024 ARM Limited.
#
# SPDX-License-Identifier: MIT
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#

add_library(
    arm_gpuman STATIC
        utils.cpp
        process.cpp
        partition.cpp
        gpu.cpp 
)

add_executable(
    gpu_manager
        main.cpp 
)

target_link_libraries(
    gpu_manager
    arm_gpuman
)

if(CMAKE_BUILD_TYPE MATCHES "Debug")
    target_compile_options(arm_gpuman PRIVATE -O0 -fsanitize=address)
    target_link_options(arm_gpuman PRIVATE -O0 -fsanitize=address -static-libasan)
    target_compile_options(gpu_manager PRIVATE -O0 -fsanitize=address)
    target_link_options(gpu_manager PRIVATE -O0 -fsanitize=address -static-libasan)
endif()

set_target_properties(arm_gpuman gpu_manager
    PROPERTIES
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
)

install(TARGETS arm_gpuman gpu_manager DESTINATION ${PACKAGE_ROOT})