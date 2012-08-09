Imagemapmaker
=============

Imagemapmaker creates XHTML image maps out of a set of image files
contained in the given direcories.

This process involves two steps:

- Create the image map images based on the original images.
- Generate the XHTML document which features the image maps
  and references the source images.

Don't forget to invoke `./imagemapmaker -h` instead of crying for help.

Installation
------------

imagemapmaker is written in C++, using STL and the gd graphics library,
so you need to fetch those dependencies.  Last but not least, type `make`.
