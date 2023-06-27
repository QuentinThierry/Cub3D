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


TO DO:

-need to fix division by zero when sin(0)
-fix get_dist return 0
-fix orientation when we have round coordonee (ex : (3.0 ; 1.0))
-make parsing
-mouvement depending on the orientation of the player
-wall collision
-organization of data
-optimization