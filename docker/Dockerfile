FROM debian:stretch

MAINTAINER franck

ENV DEBIAN_FRONTEND=noninteractive
ENV LD_LIBRARY_PATH /lib:/usr/lib:/usr/local/lib

RUN apt-get update && apt-get install -y \
	pkg-config \
	autoconf \
	automake \
	build-essential \
	linux-headers-`uname -r` \
	libssl-dev \
	make \
	libjansson4 \ 
	libjansson-dev \
 	libtool \
	tar \
	wget \
	libgtk2.0-dev \
	cgdb \
	libcurl4-openssl-dev \
	libevent-dev \
	ca-certificates \
	checkinstall \
	intltool

ADD build.sh /opt/
ADD entrypoint /opt/

RUN chmod +x /opt/build.sh && chmod +x /opt/entrypoint
ENTRYPOINT [ "/opt/entrypoint" ]