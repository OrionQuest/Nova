Getting Started
===============

The Nova library can be downloaded from `GitHub <https://github.com/>`_ using
the following command: ::

    git clone https://github.com/OrionQuest/Nova.git

Example projects that use the Nova core library live in a separate repository,
and can be downloaded using the following commands: ::

    cd Nova/Projects
    git clone https://github.com/OrionQuest/Nova_Examples.git

Nova depends on several libraries such as `GLM <https://glm.g-truc.net/0.9.9/index.html>`_,
`FreeType <https://www.freetype.org/>`_, `GLFW <http://www.glfw.org/>`_ (version ``3.0``), `GLEW <http://glew.sourceforge.net/>`_, `Boost <https://www.boost.org/>`_
(in particular, ``filesystem``, ``program_options``, and ``regex``), `Assimp <http://www.assimp.org/>`_, etc. Most of these libraries can be directly installed from the
package manager on Linux systems such as Ubuntu (version ``14.04`` or higher).
At the moment, the ``cmake`` files do not do a perfect job in finding all the missing libraries. We apologize for this slight inconvenience and will hopefully fix this in the near future.
Run the following command to install all the dependencies: ::

    sudo apt-get install libboost1.58-dev libboost-program-options1.58-dev libboost-filesystem1.58-dev libboost-regex1.58-dev libglfw3-dev libglew-dev libglm-dev libassimp-dev libfreetype6-dev

The build system uses `cmake <https://cmake.org/>`_ (version ``3.0`` or higher).
We recommend the use of the graphical version ``ccmake`` for easy configuration
of the environment variables. The best way to install the latest version of
``cmake`` and ``ccmake`` is to first run the following command: ::

    sudo apt-get install libncurses5-dev

and then install `cmake <https://cmake.org/>`_. This will ensure that both
``cmake`` and ``ccmake`` are updated to the latest version. To compile Nova, run the following commands in order: ::

    cd Nova
    mkdir build
    cd build
    ccmake ..

Set ``CMAKE_BUILD_TYPE`` to ``Release``, and turn on the following flags:
``ENABLE_ASSIMP_PLUGIN``, ``ENABLE_FREETYPE_PLUGIN``, ``ENABLE_GRID_PLUGIN``,
``ENABLE_TEXTFILERENDERER_PLUGIN``. As our first example, we will simulate
volumetric elastic objects, so turn on the corresponding project using the
flag: ``ENABLE_EMBEDDED_DEFORMABLES``. Press ``c`` to configure, which will
bring up the additional flag ``ENABLE_EMBEDDED_DEFORMABLES_PLUGIN`` used by the OpenGL viewer for visualizing the output data. Turn this flag on as well, press ``c`` to configure, and then ``g``
to generate the ``Makefile``. Finally, run the following command: ::

    make -j 8
