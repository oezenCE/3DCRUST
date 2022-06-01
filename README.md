# 3D CRUST SOFiSTiK Post-Processor
This library makes use of the C++ interface provided by SOFiSTiK to access and read results generated from CDB files, as well as the functionality of the vtu11 library to convert the FEM data to an unstructured grid (VTU file) under the VTK standard, as well as using gotraceit library to calculate and trace principal stress trajectories.
## Prerequisites
### OS
**Windows-Only**
* This software works only on Windows OS due to used commercial libraries (`sof_cdb_w-70.dll` From Sofistik FEA). Thus, necessary tools mentioned below should be deployed on a Windows OS.

### SOFiSTiK FEA
This software has been tested with SOFiSTiK FEA 2020.

It is assumed that the user has installed beforehand a running version of SOFiSTiK FEA software. Both commercial and educational versions should be supported. 
* If commercial version is used, use `sof_cdb_w-70.dll`
* If educational version is used, use `sof_cdb_w_edu-70.dll`

### VTK Libraries 
VTK development libraries installation is required(on Windows).
<br> See: [VTK Libraries Installation Instructions](https://vtk.org/Wiki/VTK/Configure_and_Build)

### Python 
If you want to use the Python bindings, make sure that Python and the Python development libraries are installed

## Installation
### Basic install
<pre>
git clone https://gitlab.lrz.de/3dcrust/3dcrust.git
cd 3dcrust
git submodule update --init --recursive
</pre>

### `sof_cdb_w-70.dll`
*Please set `SOFISTIK_DIR` on *CMakeLists* for `sof_cdb_w-70.dll` (or `sof_cdb_w-70.dll`).

<pre>
cmake_minimum_required( VERSION 3.4.3 )

project( 3DCRUST_Sofistik_Post_Processor CXX )

...

set(SOFISTIK_DIR "**Sofistik Installation Path**")
set(SOFISTIK_LIB_DIR "${SOFISTIK_DIR}\\interfaces\\64bit")
set(SOFISTIK_INCLUDE_DIR "${SOFISTIK_DIR}\\interfaces\\examples\\c++")

...
</pre>


### Enabling VTU support
*It is required to activate VTU support. Please enable VTU support for gotraceit library while configuring with CMake. Make sure that CMake gets the correct path for VTK librares.

### Enabling Python bindings
*If you want to use the Python bindings, make sure that it is enabled for both 3DCRUST and gotraceit libraries while configuring with CMake.


*** *After configuring and generating with CMake, please build **INSTALL**. Python scripts, dlls, Python wrappers all will be on install folder.*

