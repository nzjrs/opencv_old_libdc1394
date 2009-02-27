Ch OpenCV package is Ch binding to OpenCV.
With Ch OpenCV package, C (or C++) programs using OpenCV C functions 
can readily run in Ch without compilation.

The latest Ch OpenCV package can be obtained from
    http://www.softintegration.com/products/toolkit/odbc/

Ch is an embeddable C/C++ interpreter for cross platform scripting,
2D/3D plotting, numerical computing and embedded scripting.
Ch is freely available from SoftIntegration, Inc.
    http://www.softintegration.com/


Release Notes
--------------------------
Ch OpenCV 2.1, April 25, 2005
1) This updated Ch OpenCV supports OpenCV beta4 for both Windows and Linux
Ch OpenCV 2.0, March 15, 2005
1) This updated Ch OpenCV supports OpenCV beta4 and Ch 5.0.
2) ALL functions in OpenCV beta4 are supported.
3) ALL demo programs in OpenCV beta4 are readily to
run interpretively in Ch without compilation.
4) Ch SDK is now bundled with Ch 5.0 for easy interface
binary C/C++ library. Ch OpenCV in this release
can be updated automatically using Ch 5.0 or above
when OpenCV is updated by running the script./pkgcreate.ch.
using command 
    make 
----
Ch OpenCV 1.2, January 16, 2004
   Support Ch 4.5
----
Ch OpenCV 1.1, August 15, 2003


Contents
--------------------------
Ch OpenCV package contains the following directories

(1) chopencv - Ch OpenCV package
chopencv/demos       ---  OpenCV demo programs in C readily to run in Ch
chopencv/bin         ---  OpenCV dynamical library and commands
chopencv/dl          ---  dynamically loaded library
chopencv/include     ---  header files
chopencv/lib         ---  function files

(2) Files and utilities used to build Ch OpenCV package
    using native C compiler and Ch SDK
c            ---  _chdl.c C wrappers and Makefile's
chfcreate    ---  bare OpenCV headers for creating .chf files
chfhandmade  ---  Ch function files in .chf for special functions 
demos        ---  demos to run in Ch 
pkgcreate.ch ---  A Ch program to update Ch OpenCV package
pkginstall.ch ---  A Ch program to install Ch OpenCV package


System Requirement 
--------------------------
(1) Ch Standard or Professional Edition version 5.0.1.12211 or higher.
    It can be downloaded from http://www.softintegration.com.
(2) OpenvCV beta 4a or higher if a newer version is to be built.


To Build Ch OpenCV
--------------------------
    This Ch OpenCV source directory should be placed at <OPENCV_HOME>/ch
    In a command shell, type
       make 


Installation Instructions:
--------------------------
(1) If Ch has not been installed in your computer,
    install Ch from http://www.softintegration.com.

(2) There are three ways for installing Ch OpenCV package. 
You can use one of three methods below.

Method 1.
    In a command shell, type

       make install 

Method 2.
(a) Move the directory ./chopencv to CHHOME/package/chopencv, 
    where CHHOME is the directory Ch installed.
(b) Assume CHHOME is C:/Ch, 
      copy ./chopencv/include/* to CHHOME/toolkit/include dir 
    or add
      _ipath = stradd(_path, "C:/Ch/package/chopencv/include;");
    in startup file _chrc, such as C:/_chrc, in your home directory.
    If the following code fragment is in applicaton programs
        #ifdef _CH_
        #pragma package <chopencv>
        #endif
    this step is not necessary.

Method 3.
(a) Assume Ch OpenCV package is installed in 
    C:/Program Files/OpenCV/ch/chopencv.
(b) Add a new path to the system variable _ppath for package 
    path by adding the following statement
    in system startup file CHHOME/config/chrc or individual 
    user's startup file ~/_chrc 
      _ppath = stradd(_ppath, "C:/Program Files/OpenCV/ch;");
    Here we assume that the Ch OpenCV package is installed in 
    the directory  C:/Program Files/OpenCV/ch/chopencv.
(c) Add a path for searching header files by adding
      _ipath = stradd(_path, "C:/Program Files/OpenCV/ch/chopencv/include;");
    in startup file _chrc, such as C:/_chrc, in your home directory.
    If the following code fragment is in applicaton programs
         #ifdef _CH_
     #pragma package <opencv>
         #endif
    this step will not be necessary.


-------------------------------------------------------------
Run Demo Programs in Directory demos:
(1) start ch
(2) type program name such as 'delaunay.c'


How to update Ch OpenCV package for newer versions of OpenCV:
-------------------------------------------------------------
(1) If Ch has not been installed in your computer,
    install Ch from http://www.softintegration.com.
(2) Run
        make 
(3) install the newly created Ch OpenCV package by command
        make install 
(4) Questions specific to using Ch SDK to update Ch OpenCV can be posted in 
    either OpenCV discussion group or Ch discussion group at
    http://www.softintegration.com.  

Uninstallation Instructions:
--------------------------
    In a command shell, type

       make uninstall 
