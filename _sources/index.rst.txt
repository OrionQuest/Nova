.. Nova documentation master file, created by
   sphinx-quickstart on Wed Aug 31 17:54:08 2016.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

Multi-Physics Simulation with Nova
==================================

Nova is a high-performance library for Computational Physics. In terms of
design, it draws inspiration from its elder brother `PhysBAM <http://physbam.stanford.edu/>`_,
but uses modern libraries such as `STL <https://en.wikipedia.org/wiki/Standard_Template_Library>`_,
`Boost <https://www.boost.org/>`_, `Assimp <http://www.assimp.org/>`_, etc.,
to reuse existing open-source implementations whenever possible. Parts of it have been (and continue to be) parallelized using
`OpenMP <https://www.openmp.org/>`_. Our goal is to make it a versatile library
for various multi-physics problems and geometric computation. This documentation provides tutorials
on how to use the library.

.. toctree::
   :maxdepth: 2
              
   Getting Started <start>
   Configuring OpenGL <viewer>
   Elasticity Simulation <elasticity>

Indices and tables
==================

* :ref:`genindex`
* :ref:`modindex`
* :ref:`search`


