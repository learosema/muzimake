# Compiler and linker settings
CC = wpp386
LD = wlink
SYSTEM=dos4g
CFLAGS=-zq -zastd=c++0x -bt=dos
LDFLAGS=

# Source files
SRCS = main.cpp textmode.cpp mouse.cpp vga.cpp adlib.cpp opl2.cpp bnkfile.cpp

!ifdef __UNIX__
O_EXT=.o
CP = cp
RM = rm
DOS4GW_RUNTIME=$(%WATCOM)/binw/dos4gw.exe
OBJS = $(SRCS:.cpp=.o)
!else
CP = copy
RM = del
DOS4GW_RUNTIME=$(%WATCOM)\binw\dos4gw.exe
OBJS = $(SRCS:.cpp=.obj)
O_EXT=.obj
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


testinst: .symbolic postbuild
	$(CC) $(CFLAGS) bnkfile.cpp instr.cpp testinst.cpp
	$(LD) system $(SYSTEM) $(LDFLAGS) name testinst.exe file { bnkfile$(O_EXT) instr$(O_EXT) testinst$(O_EXT) }

test_bnk: .symbolic postbuild
	$(CC) $(CFLAGS) test_opl.cpp bnkfile.cpp opl2.cpp
	$(LD) system $(SYSTEM) $(LDFLAGS) name test_bnk.exe file { test_bnk$(O_EXT) bnkfile$(O_EXT) opl2$(O_EXT) }

test_rol: .symbolic postbuild
	$(CC) $(CFLAGS) bnkfile.cpp opl2.cpp file.cpp rolfile.cpp test_rol.cpp
	$(LD) system $(SYSTEM) $(LDFLAGS) name test_rol.exe file { test_rol$(O_EXT) bnkfile$(O_EXT) opl2$(O_EXT) file$(O_EXT) rolfile$(O_EXT) }

# Clean rule
clean: .symbolic
	$(RM) *.o
	$(RM) *.obj
	$(RM) *.OBJ
	$(RM) *.exe
	$(RM) *.EXE
