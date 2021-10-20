TriLib
=======
Library for advanced 2D and 3D math
-----------------------------------

To compile the library execute:

```
cd TriLib
mkdir build/win64
cd build/win64

cmake ../.. -G "<INSERT_BUILDSYSTEM_HERE>"
make
```

Make sure to install ``cmake,make,gcc`` for this to work.

Credits
-------
https://gist.github.com/hideo55/5642892 is used to check wether SSE2 is supported on the host system