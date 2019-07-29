set OPENCV_TOP=\p
set OPENCV_INC=%OPENCV_TOP%\opencv\build\include
set OPENCV_LIB=%OPENCV_TOP%\opencv\build\x64\vc15\lib
set OPENCV_DIR=%OPENCV_TOP%\opencv\build\x64\vc15\bin
set CXXFLAGS=/Zi /Ox /EHsc /DNDEBUG /DNOMINMAX /W3 -I %OPENCV_INC% -I ../mcl/include -I../cybozulib/include -I../xbyak/ -I../cybozulib_ext/include
set LFLAGS=/link /LIBPATH:%OPENCV_LIB% /LIBPATH:../mcl/lib /LIBPATH:../cybozulib_ext/lib
cl %CXXFLAGS% server.cpp %LFLAGS%
cl %CXXFLAGS% main.cpp %LFLAGS%
