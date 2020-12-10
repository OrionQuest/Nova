Configuring OpenGL
==================

The OpenGL viewer (available in ``Nova_Examples/opengl``) requires some symbolic links
to be placed in the ``build`` directory for proper configuration. Run the
following commands in order: ::

    cd build/
    ln -s ../Projects/Nova_Examples/opengl/example/nova.conf .
    ln -s ../Projects/Nova_Examples/opengl/example/fonts/ .
    ln -s ../Projects/Nova_Examples/opengl/plugins/Grid/shaders/ .

The viewer uses the modern GL pipeline and supports a plugin-based architecture, in the sense that every project
can define its own plugin on how to view the generated data. While this does
place the burden on the developer to write a separate plugin per project, it
has the nice benefit of moving away from hard coding things in some
master file.
