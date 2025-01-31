# Compiler and linker settings
CC = wpp386
LD = wlink
SYSTEM=dos4g
CFLAGS=-zq -zastd=c++0x -bt=dos
LDFLAGS=

!ifdef __UNIX__
O_EXT=o
CP=cp
RM=rm -f
RD=rm -rf
DOS4GW_RUNTIME=$(%WATCOM)/binw/dos4gw.exe
SRC_DIR=./src/
BIN_DIR=./bin/
TEST_DIR=./tests/
MD_BIN=mkdir -p $(BIN_DIR)
!else
CP=copy
RM=del
RD=rd
DOS4GW_RUNTIME=$(%WATCOM)\binw\dos4gw.exe
O_EXT=obj
SRC_DIR=.\src\
BIN_DIR=.\bin\
TEST_DIR=.\tests\
MD_BIN=if not exist $(BIN_DIR) md $(BIN_DIR)
!endif

SRCS=main.cpp textmode.cpp mouse.cpp vga.cpp adlib.cpp instr.cpp opl2.cpp bnkfile.cpp
TEST_ROL_SRCS=instr.cpp bnkfile.cpp opl2.cpp file.cpp rolfile.cpp test_rol.cpp
TEST_BNK_SRCS=opl2.cpp instr.cpp bnkfile.cpp test_bnk.cpp
TESTINST_SRCS=bnkfile.cpp instr.cpp testinst.cpp

OBJS = $(SRCS:.cpp=.o)
TEST_ROL_OBJS=$(TEST_ROL_SRCS:.cpp=.o)
TEST_BNK_OBJS=$(TEST_BNK_SRCS:.cpp=.o)
TESTINST_OBJS=$(TESTINST_SRCS:.cpp=.o)

# Executable name
TARGET = muzimake.exe

TESTS = testinst.exe test_rol.exe test_bnk.exe

# Default target
all: prebuild $(TARGET) postbuild .symbolic

tests: prebuild $(TESTS) postbuild .symbolic

prebuild: .symbolic
	$(MD_BIN)

postbuild: .symbolic
	$(CP) $(DOS4GW_RUNTIME) $(BIN_DIR)

.o: $(BIN_DIR)
.obj: $(BIN_DIR)
.exe: $(BIN_DIR)
.cpp: $(SRC_DIR);$(TESTS_DIR)

.cpp.$(O_EXT):
	$(CC) $[@ -fo=$(BIN_DIR)$[&.$(O_EXT) $(CFLAGS)

# Link rule for the executable
$(TARGET): $(OBJS)
	$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)$(TARGET) file { $< }

testinst.exe: $(TESTINST_OBJS)
	$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)testinst.exe file { $< }

test_bnk.exe: $(TEST_BNK_OBJS)
	$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)test_bnk.exe file { $< }

test_rol.exe: $(TEST_ROL_OBJS)
	$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)test_rol.exe file { $< }

# Clean rule
clean: .symbolic
	$(RM) $(BIN_DIR)*.*
	$(RD) $(BIN_DIR)
