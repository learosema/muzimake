# Compiler and linker settings
CC = owcc
LD = owcc
CFLAGS = -zq -c -xc++ -bdos4g
LDFLAGS = -zq -bdos4g
CP = copy
RM = del
!ifdef __UNIX__
DOS4GW_RUNTIME=$(%WATCOM)/binw/dos4gw.exe
!else
DOS4GW_RUNTIME=$(%WATCOM)\binw\dos4gw.exe
!endif

# Source files
SRCS = main.cpp textmode.cpp mouse.cpp vga.cpp adlib.cpp

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

# Clean rule
clean: .symbolic
	$(RM) *.obj
	$(RM) *.exe
