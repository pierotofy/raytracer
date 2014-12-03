Raytracer
=========
This is a simple Ray Tracer written in C++.

![Alt text](/output_examples/bneumann_00_2011.png?raw=true "")

Planet animation: http://youtu.be/6G5qIkZhATA

Transformations animation: http://youtu.be/SCoP93IcWRI

Features
=========
- OBJ files rendering
- Lambertian, blinnphong, normal and “glaze” shading
- Basic geometries for spheres, cylinders, cubes and triangles
- Reflections
- Soft shadows
- Rough surfaces
- Texture mapping
- Skymaps
- Transformations
- BVH acceleration
- Multi-threaded rendering
- Point and area lighting
- Animation and stereographic (3D) rendering

Build instructions (Linux only)
=========
To compile first run the /src/install_requirements.sh script (will load the dependencies using apt-get). Then from the /bin directory type:

```
cmake ../src
make
./raytracer
```

(C) Piero Toffanin 2012
