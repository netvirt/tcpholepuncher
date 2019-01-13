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

* Build the project with docker on Linux
```
cd tcpholepuncher/docker
make build
make run
make compile
make punch
```


