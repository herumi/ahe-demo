include ../mcl/common.mk

TARGET=server.exe main.exe

all: $(TARGET)

MCL_LIB=../mcl/lib/libmcl.a
$(MCL_LIB):
	$(MAKE) -C ../mcl

CFLAGS+=-I../mcl/include
LDFLAGS+=-lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_videoio -lcrypto -lssl

ALL_SRC=server.cpp main.cpp
server.o: server.cpp


%.o: %.cpp
	$(PRE)$(CXX) $(CFLAGS) -c $< -o $@ -MMD -MP -MF $(@:.o=.d)
%.exe: %.o $(MCL_LIB)
	$(PRE)$(CXX) $< -o $@ -lmcl -L../mcl/lib $(LDFLAGS)

clean:
	rm -rf *.exe *.o *.d

DEPEND_FILE=$(ALL_SRC:.cpp=.d)
-include $(DEPEND_FILE)

# don't remove these files automatically
.SECONDARY: $(ALL_SRC:.cpp=.o)
