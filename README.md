# About

libGPUMan is a small library that allows to monitor Arm® GPU activity and configure partitions enabled through virtualization.

This library is able to query processes using GPU resources and report memory usage. 

If virtualization is enabled, this library is also able to report the resources allocated (e.g. number of GPU slices) to the different partitions and configure them. It is tightly coupled to the [Arm Mali GPUs Arbitration Reference Code][1], which interface is subject to change. This library is provided as a demonstrator.

For more information about GPU virtualization, please refer to [Arm Immortalis and Mali GPU Virtualization Guide][2].

## Supported devices

The library was developed for devices with Arm Mali™ GPU running Linux.

## Features

### Monitoring

The library collects the following information about the GPU:
- Model name, revision and number of shader cores,
- Mali DDK driver version,
- Number of partitions,
- Available system memory,
- Total GPU memory usage.

For each partition found, the library collects:
- Status (e.g. active, suspended)
- Allocated slice IDs,
- Assigned access window,
- GPU memory usage,
- List of running processes.

For each running process, the library collects:
- PID,
- Command,
- GPU memory usage.

### Configuration

The library enables to dynamically set the following for any partition:
- allocated slice IDs,
- assigned access window.

## Building

Use the following commands:
```
cmake -B build/
cmake --build build/
```

The library will be build statically in `build/lib` and an example CLI tool called `gpu_manager` in `build/bin`.

## Using the library

An example of a CLI tool `gpu_manager` is provided with the library in `main.cpp`.

```
./gpu_manager --help
Arm Mali GPU monitoring tool
Usage: ./gpu_manager [-h|--help] [-y|--yaml] [-s|--slices PARTITION:SLICES] [-a|--access_window PARTITION:AW]
  Monitoring mode:
    -h/--help: print this help and exit
    -y/--yaml: output in YAML format
    -u/--update: automatically update every second
  Configuration mode:
    -s/--slices: assign hex value SLICES to partition PARTITION
    -a/--access_window: assign hex value AW to partition PARTITION
```

- - -

_Copyright © 2024, Arm Limited and contributors. All rights reserved._

[1] https://developer.arm.com/downloads/-/mali-drivers/mali-gpus-arbitration-reference-code
[2] https://developer.arm.com/documentation/102104/latest/