cd build
make clean
cmake ..
make -j4
./A1 ../resources/tri.obj out.png 600 600 1
