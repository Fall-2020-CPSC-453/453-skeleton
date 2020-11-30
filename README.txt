Assignment 4 boiler plate. This boilerplate is quite a bit different than your previous ones.

First, you cannot change the view of the scene with keyboard and mouse. To do that, you will need to edit the code and ensure that the camera is setup appropriately.

Second, there are two scenes. You can switch between them with the keys 1, and 2.

There are a bunch of new files:

* Material.h/Material.cpp provide a struct/class to describe the material properties of objects.
* Lighting.h implements the phong shading model from lecture which already shades objects for you.
* RayTrace.h/RayTrace.cpp provides a Ray class, an abstract Shape base class and other shape classes that inherit from it, including Triangles, Plane and Sphere.  This uses your typical inheritance model to ensure that you can deal with a vector of heterogenous shapes.
* Scene.h/Scene.cpp defines the two scenes.
* imagebuffer.h/imagebuffer.cpp - Translates your image to / from OpenGL and allows you to save the image to disk.

Files you need to change:
1. main.cpp has TODO comments in each of the places you need to change it. Parts 1, 3 and 4 need to be implemented here.
2. RayTrace.cpp has a TODO comment in the one place you need to change it. Part 2 is implemented here.


