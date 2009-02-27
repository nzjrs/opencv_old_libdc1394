#!/bin/sh
./configure 			\
	--disable-apps		\
	--with-python=no 	\
	--with-swig=no 		\
	--with-octave=no 	\
	--with-unicap=no 	\
	--with-v4l=yes 		\
	--with-quicktime=no	\
	--with-ffmpeg=yes	\
	--with-gstreamer=no	\
	--with-xine=no		\
	--with-gtk=yes		\
	--with-gthread=yes	\
	--with-1394libs=no	\
	--prefix=/usr/local

