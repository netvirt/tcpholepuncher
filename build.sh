#/bin/sh
export LD_LIBRARY_PATH=/lib:/usr/lib:/usr/local/lib

autoreconf -fiv &&\
./configure &&\
make &&\
make install 


