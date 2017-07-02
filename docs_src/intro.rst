Introduction to the Nova Library
================================

Welcome to Nova, a high performance computational library that provides support for
simulation, rendering and visualization. Nova is written primarily in C++ and supports all
the latest open-source libraries and file formats for handling different kinds
of data sets such as meshes, images, or volumes. Its unique design features a modular
architecture for ease of use in geometric modeling and simulation, along with a plug-in
based infrastructure that allows easy visualization of custom data structures.
The library supports cross-platform compilation with ``gcc`` and ``clang`` and has been
tested on Linux and Mac OS X operating systems. Several parts of the library
have also been extensively optimized using `OpenMP <http://www.openmp.org/>`_
parallelization. It provides support for simulating rigid body dynamics,
compressible and incompressible fluids, volumetric elastic solids,
and multi-material interactions.

The library requires `cmake <https://cmake.org/>`_ (version ``3.0`` or higher).
Use of the graphical version ``ccmake`` is recommended for easy
configuration of environment variables.
To compile, create a ``build`` directory and run the following commands: ::

    mkdir build
    cd build
    ccmake ..
