set OPENCV_INC=\p\opencv3\build\include
set OPENCV_LIB=\p\opencv3\build\x64\vc14\lib
set CXXFLAGS=/Zi /Ox /EHsc /DNDEBUG /DNOMINMAX /W3 -I %OPENCV_INC% -I ../mcl/include -I../cybozulib/include -I../xbyak/ -I../cybozulib_ext/include
set LFLAGS=/link /LIBPATH:%OPENCV_LIB% /LIBPATH:../mcl/lib /LIBPATH:../cybozulib_ext/lib
cl %CXXFLAGS% server.cpp %LFLAGS%
cl %CXXFLAGS% main.cpp %LFLAGS%
