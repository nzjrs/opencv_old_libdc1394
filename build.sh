#!/bin/sh

# Dependencies
# apt-get build-dep opencv
# libdc1393-2.1
# libv4l-dev

./configure --with-dc1394v2=yes --with-v4l=yes --with-quicktime=no --with-ffmpeg=no --with-python=no --enable-apps

