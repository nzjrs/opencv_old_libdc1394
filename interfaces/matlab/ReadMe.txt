This is a source code and executables for MATLAB wrappers for OpenCV.
To install wrappers, set path in the MATLAB to the "toolbox/opencv"
subfolder of this folder and, optionally, to "toolbox/opencv/demos" subfolder.
Wrappers for OpenCV functions can be run in two ways:
cvwrap('<FunctionName>', parameters) where FunctionName is opencv function without
cv prefix,
or
just cvFunctionName( parameters).

To build matlab wrappers you need to add <matlab_folder>/extern/include folder
to include path (within Developer Studio) and
<matlab_folder>/extern/lib/win32;<matlab_folder>/extern/lib/win32/microsoft/msvc60
folders to lib path.

Note, that the interface is not complete, only a few essential functions are
wrapped.
