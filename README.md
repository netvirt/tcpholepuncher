* Build the project
```
git clone git@github.com:0xFranckx0/tcpholepuncher.git
cd tcpholepuncher/docker
make build
make run
make compile
make punch
```

* Build the library with cmake
```
mkdir build.lib
cd build.lib
cmake ..
make
```

* Build the library with cmake on Windows
```
mkdir build.lib
cd build.lib
cmake -DCMAKE_TOOLCHAIN_FILE=win32/toolchain-mingw32.cmake ..
make
```
