# Compiler and linker settings
CC = owcc
LD = owcc
CFLAGS = -zq -std=c\\9\\9 -c -xc++ -bdos4g
LDFLAGS = -zq -bdos4g
!ifdef __UNIX__
CP = cp
RM = rm
DOS4GW_RUNTIME=$(%WATCOM)/binw/dos4gw.exe
!else
CP = copy
RM = del
DOS4GW_RUNTIME=$(%WATCOM)\binw\dos4gw.exe
!endif

# Source files
SRCS = main.cpp textmode.cpp mouse.cpp vga.cpp adlib.cpp opl2.cpp bnkfile.cpp

# Object files
OBJS = $(SRCS:.cpp=.obj)

# Executable name
TARGET = muzimake.exe

# Default target
all: $(TARGET) postbuild .symbolic

postbuild: .symbolic
	$(CP) $(DOS4GW_RUNTIME) .

# Build rule for object files
.cpp.obj: %.cpp
	$(CC) $(CFLAGS) -o $@ $<

# Link rule for the executable
$(TARGET): $(OBJS)
	$(LD) $(LDFLAGS) -o $@ $(OBJS)

test: .symbolic
	$(CC) $(CFLAGS) -o test.obj test.cpp
	$(CC) $(CFLAGS) -o bnkfile.obj bnkfile.cpp
	$(LD) $(LDFLAGS) -o test.exe test.obj bnkfile.obj

# Clean rule
clean: .symbolic
	$(RM) *.obj
	$(RM) *.exe
