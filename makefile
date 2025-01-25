# Compiler and linker settings
CC = wpp386
LD = wlink
SYSTEM=dos4g
CFLAGS=-zq -zastd=c++0x -bt=dos
LDFLAGS=

# Source files
SRCS = main.cpp textmode.cpp mouse.cpp vga.cpp adlib.cpp opl2.cpp bnkfile.cpp

!ifdef __UNIX__
OBJEXT=.o
CP = cp
RM = rm
DOS4GW_RUNTIME=$(%WATCOM)/binw/dos4gw.exe
OBJS = $(SRCS:.cpp=.o)
!else
CP = copy
RM = del
DOS4GW_RUNTIME=$(%WATCOM)\binw\dos4gw.exe
OBJS = $(SRCS:.cpp=.obj)
OBJEXT=.obj
!endif

# Executable name
TARGET = muzimake.exe

# Default target
all: $(TARGET) postbuild .symbolic

postbuild: .symbolic
	$(CP) $(DOS4GW_RUNTIME) .

# Build rule for object files
!ifdef __UNIX__
.cpp.o: %.cpp
	$(CC) $(CFLAGS) $<
!else
.cpp.obj: %.cpp
	$(CC) $(CFLAGS) $<
!endif

# Link rule for the executable
$(TARGET): $(OBJS)
	$(LD) system $(SYSTEM) $(LDFLAGS) name $(TARGET) file { $(OBJS) }

test: .symbolic postbuild
	$(CC) $(CFLAGS) test.cpp bnkfile.cpp opl2.cpp
	$(LD) system $(SYSTEM) $(LDFLAGS) name test.exe file { test$(OBJEXT) bnkfile$(OBJEXT) opl2$(OBJEXT) }

test_rol: .symbolic postbuild
	$(CC) $(CFLAGS) bnkfile.cpp opl2.cpp byteswap.cpp file.cpp rolfile.cpp test_rol.cpp
	$(LD) system $(SYSTEM) $(LDFLAGS) name test_rol.exe file { test_rol$(OBJEXT) bnkfile$(OBJEXT) opl2$(OBJEXT) rolfile$(OBJEXT) file$(OBJEXT) byteswap$(OBJEXT) }

# Clean rule
clean: .symbolic
	$(RM) *.o
	$(RM) *.obj
	$(RM) *.OBJ
	$(RM) *.exe
	$(RM) *.EXE
