# Cub3D

first commit

compilation avec son
gcc ao_example.c libao.so.4.1.0 -lm -I.


tan(0) ou 90 => seg fault

install libao :

curl -L http://downloads.xiph.org/releases/ao/libao-1.2.0.tar.gz --output libao

tar -xf libao

./configure --prefix=/mnt/nfs/homes/jvigny/Documents/42/test --exec-prefix=/mnt/nfs/homes/jvigny/Documents/42/test

make

make install



Dimension of the Projection Plane = WIN_X * WIN_Y units
Center of the Projection Plane = (WIN_X / 2 , WIN_Y / 2)
Distance to the Projection Plane = (WIN_X/2.0) / (t(FOV/2.0 in radian))
Angle between subsequent rays = FOV/WIN_X degree

