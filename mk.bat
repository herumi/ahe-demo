set OPENCV_TOP=.\
set OPENCV_INC=%OPENCV_TOP%\opencv\build\include
set OPENCV_LIB=%OPENCV_TOP%\opencv\build\x64\vc15\lib
set OPENCV_DIR=%OPENCV_TOP%\opencv\build\x64\vc15\bin
set CXXFLAGS=/Zi /Ox /EHsc /DNDEBUG /DNOMINMAX /W3 -I %OPENCV_INC% -I ../mcl/include /DMCL_USE_VINT /DMCL_DONT_USE_OPENSSL /DMCL_VINT_FIXED_BUFFER /DMCL_MAX_BIT_SIZE=384 /openmp
set LFLAGS=/link /LIBPATH:%OPENCV_LIB% /LIBPATH:../mcl/lib
cl %CXXFLAGS% server.cpp %LFLAGS%
cl %CXXFLAGS% main.cpp %LFLAGS%
