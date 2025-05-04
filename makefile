# Compiler and linker settings
CXX=wpp386
LD=wlink
SYSTEM=dos4g
CFLAGS=-zq -zastd=c++0x -bt=dos -dDEBUG_LOGS -d2
LDFLAGS=option quiet

SRC_DIR=src
BIN_DIR=bin
OBJ_DIR=obj
TEST_DIR=tests
VENDOR_DIR=vendor

SRC_DIR_EVT=$(SRC_DIR)/events
SRC_DIR_FMT=$(SRC_DIR)/filefmt
SRC_DIR_HAL=$(SRC_DIR)/hal
SRC_DIR_HLP=$(SRC_DIR)/helper
SRC_DIR_TUI=$(SRC_DIR)/textui

CXX_INCL=-i=$(SRC_DIR) -i=$(SRC_DIR_EVT) -i=$(SRC_DIR_FMT) -i=$(SRC_DIR_HAL) -i=$(SRC_DIR_HLP) -i=$(SRC_DIR_TUI) -i=$(VENDOR_DIR)

!ifdef __UNIX__
O_EXT=o
RM=rm -f
RD=rm -rf
SEP=/
MD_BIN=mkdir -p $(BIN_DIR)
MD_OBJ=mkdir -p $(OBJ_DIR)
!else
RM=del
RD=rd
SEP=\
O_EXT=obj
MD_BIN=if not exist bin md bin
MD_OBJ=if not exist $(OBJ_DIR) md $(OBJ_DIR)
!endif

SRC_EVENTS=events.cpp
SRC_FILEFMT=file.cpp fileio.cpp bnkfile.cpp rolfile.cpp
SRC_HAL=instrmnt.cpp opl2.cpp
SRC_HELPER=list.cpp
SRC_TEXTUI=mouse.cpp dpmiutil.cpp textmode.cpp vga.cpp

SRCS=main.cpp $(SRC_EVENTS) $(SRC_FILEFMT) $(SRC_HAL) $(SRC_HELPER) $(SRC_TEXTUI)

TEST_COMMON_SRCS=bnkfile.cpp rolfile.cpp fileio.cpp file.cpp

TEST_EVT_SRCS=events.cpp test_evt.cpp
LIB_UI_SRCS=dpmiutil.cpp mouse.cpp textmode.cpp ui_event.cpp ui_common.cpp ui_label.cpp ui_btn.cpp ui_input.cpp ui_lsbox.cpp ui_piano.cpp ui_range.cpp ui_sheet.cpp cmponent.cpp
TEST_ROL_SRCS=$(TEST_COMMON_SRCS) instrmnt.cpp opl2.cpp test_rol.cpp
TEST_BNK_SRCS=$(TEST_COMMON_SRCS) instrmnt.cpp opl2.cpp test_bnk.cpp
TESTINST_SRCS=$(TEST_COMMON_SRCS) instrmnt.cpp testinst.cpp
TEST_TMR_SRCS=dpmiutil.cpp textmode.cpp timer.cpp test_tmr.cpp
TEST_KBD_SRCS=dpmiutil.cpp textmode.cpp mouse.cpp test_kbd.cpp
TEST_MOU_SRCS=test_mou.cpp
TEST_UI_SRCS=$(LIB_UI_SRCS) log.cpp test_ui.cpp
TEST_SRCS    =$(TEST_COMMON_SRCS) instrmnt.cpp test.cpp

OBJS=$(SRCS:.cpp=.$(O_EXT))

TEST_EVT_OBJS=$(TEST_EVT_SRCS:.cpp=.$(O_EXT))
TEST_ROL_OBJS=$(TEST_ROL_SRCS:.cpp=.$(O_EXT))
TEST_BNK_OBJS=$(TEST_BNK_SRCS:.cpp=.$(O_EXT))
TESTINST_OBJS=$(TESTINST_SRCS:.cpp=.$(O_EXT))
TEST_TMR_OBJS=$(TEST_TMR_SRCS:.cpp=.$(O_EXT))
TEST_MOU_OBJS=$(TEST_MOU_SRCS:.cpp=.$(O_EXT))
TEST_KBD_OBJS=$(TEST_KBD_SRCS:.cpp=.$(O_EXT))
TEST_UI_OBJS=$(TEST_UI_SRCS:.cpp=.$(O_EXT))
TEST_OBJS=$(TEST_SRCS:.cpp=.$(O_EXT))

# Executable name
TARGET = muzimake.exe

TESTS = testinst.exe test_evt.exe test_rol.exe test_bnk.exe test_ui.exe test_tmr.exe test_kbd.exe test_mou.exe test.exe

# Default target
.erase
all: prebuild $(TARGET) postbuild .symbolic

.erase
tests: prebuild $(TESTS) postbuild .symbolic

prebuild: .symbolic
	$(MD_OBJ)
	$(MD_BIN)

postbuild: .symbolic
	%copy $(%WATCOM)/binw/dos4gw.exe $(BIN_DIR)
	%copy $(TEST_DIR)/STANDARD.BNK $(BIN_DIR)
	%copy $(TEST_DIR)/STARTREK.ROL $(BIN_DIR)

.o: $(OBJ_DIR)
.obj: $(OBJ_DIR)
.exe: $(BIN_DIR)
.cpp: $(SRC_DIR);$(SRC_DIR_EVT);$(SRC_DIR_FMT);$(SRC_DIR_HAL);$(SRC_DIR_HLP);$(SRC_DIR_TUI);$(TEST_DIR)

.cpp.$(O_EXT):
	*$(CXX) $[@ $(CXX_INCL) -fo=$(OBJ_DIR)/$[&.$(O_EXT) $(CFLAGS)

# Link rule for the executable
$(TARGET): $(OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/$(TARGET) file { $< }

test.exe: $(TEST_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test.exe file { $< }

testinst.exe: $(TESTINST_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/testinst.exe file { $< }

test_bnk.exe: $(TEST_BNK_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_bnk.exe file { $< }

test_evt.exe: $(TEST_EVT_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_evt.exe file { $< }

test_rol.exe: $(TEST_ROL_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_rol.exe file { $< }

test_ui.exe: $(TEST_UI_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_ui.exe file { $< }

test_tmr.exe: $(TEST_TMR_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_tmr.exe file { $< }

test_kbd.exe: $(TEST_KBD_OBJS)
	*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_kbd.exe file { $< }

test_mou.exe: $(TEST_MOU_OBJS)
		*$(LD) system $(SYSTEM) $(LDFLAGS) name $(BIN_DIR)/test_mou.exe file { $< }

# Clean rule
clean: .symbolic
	$(RM) $(BIN_DIR)$(SEP)*.*
	$(RD) $(BIN_DIR)
	$(RM) $(OBJ_DIR)$(SEP)*.*
	$(RD) $(OBJ_DIR)
	$(RM) *.err
