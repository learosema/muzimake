# Compiler and linker settings
CC = wpp386
LD = wlink
SYSTEM=dos4g
CFLAGS=-zq -zastd=c++0x
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
	$(CC) $(CFLAGS) test.cpp bnkfile.cpp
	$(LD) system $(SYSTEM) $(LDFLAGS) name test.exe file { test$(OBJEXT) bnkfile$(OBJEXT) }

# Clean rule
clean: .symbolic
	$(RM) *.o
	$(RM) *.obj
	$(RM) *.OBJ
	$(RM) *.exe
	$(RM) *.EXE
