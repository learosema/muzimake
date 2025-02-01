# Compiler and linker settings
CXX=wpp386
LD=wlink
SYSTEM=dos4g
CFLAGS=-zq -zastd=c++0x -bt=dos
LDFLAGS=

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj
TEST_DIR=tests

!ifdef __UNIX__
O_EXT=o
CP=cp
RM=rm -f
RD=rm -rf
SEP=/
MD_BIN=mkdir -p $(BIN_DIR)
MD_OBJ=mkdir -p $(OBJ_DIR)
!else
CP=copy
RM=del
RD=rd
SEP=\
O_EXT=obj
MD_BIN=if not exist bin md bin
MD_OBJ=if not exist $(OBJ_DIR) md $(OBJ_DIR)
!endif

SRCS=main.cpp textmode.cpp mouse.cpp vga.cpp adlib.cpp instr.cpp opl2.cpp bnkfile.cpp
TEST_ROL_SRCS=instr.cpp bnkfile.cpp opl2.cpp file.cpp rolfile.cpp test_rol.cpp
TEST_BNK_SRCS=opl2.cpp instr.cpp bnkfile.cpp test_bnk.cpp
TESTINST_SRCS=bnkfile.cpp instr.cpp testinst.cpp

OBJS = $(SRCS:.cpp=.$(O_EXT))
TEST_ROL_OBJS=$(TEST_ROL_SRCS:.cpp=.$(O_EXT))
TEST_BNK_OBJS=$(TEST_BNK_SRCS:.cpp=.$(O_EXT))
TESTINST_OBJS=$(TESTINST_SRCS:.cpp=.$(O_EXT))

# Executable name
TARGET = muzimake.exe

TESTS = testinst.exe test_rol.exe test_bnk.exe

# Default target
all: prebuild $(TARGET) postbuild .symbolic

tests: prebuild $(TESTS) postbuild .symbolic

prebuild: .symbolic
	$(MD_OBJ)
	$(MD_BIN)

postbuild: .symbolic
	$(CP) $(%WATCOM)$(SEP)binw$(SEP)dos4gw.exe $(BIN_DIR)

.o: $(OBJ_DIR)
.obj: $(OBJ_DIR)
.exe: $(BIN_DIR)
.cpp: $(SRC_DIR);$(TEST_DIR)

.cpp.$(O_EXT):
	$(CXX) $[@ -i=$(SRC_DIR) -fo=$(OBJ_DIR)/$[&.$(O_EXT) $(CFLAGS)

# Link rule for the executable
$(TARGET): $(OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/$(TARGET) file { $< }

testinst.exe: $(TESTINST_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/testinst.exe file { $< }

test_bnk.exe: $(TEST_BNK_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_bnk.exe file { $< }

test_rol.exe: $(TEST_ROL_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_rol.exe file { $< }

# Clean rule
clean: .symbolic
	$(RM) $(BIN_DIR)$(SEP)*.*
	$(RD) $(BIN_DIR)
	$(RM) $(OBJ_DIR)$(SEP)*.*
	$(RD) $(OBJ_DIR)
