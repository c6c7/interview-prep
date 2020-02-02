CC=clang

TARGET_EXEC ?= ${name}

BUILD_DIR ?= ./build/${TARGET_EXEC}
SRC_DIRS ?= $(addprefix ./src/,${TARGET_EXEC} cystring multi-stack hanoi)

SRCS := $(shell find $(SRC_DIRS) -name *.c -or -name *.s)
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)
DEPS := $(OBJS:.o=.d)

INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_FLAGS := $(addprefix -I,$(INC_DIRS))


ifeq ($(origin CPPFLAGS), undefined)

CPPFLAGS += -std=c17 # Use the the C17 Standard
# From feature_test_macros(7):
# (Since glibc 2.10) The value 200809L or
# greater additionally exposes definitions
# corresponding to the POSIX.1-2008 base
# specification (excluding the XSI extension).
CPPFLAGS += -D_POSIX_C_SOURCE=200809L

# Uncomment to enable debugging
CPPFLAGS += -g
## Uncomment to save intermediate files during compilation
#CPPFLAGS += -save-temps

CPPFLAGS += $(INC_FLAGS)
CPPFLAGS += -MMD
CPPFLAGS += -MP

endif

$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CC) $(OBJS) -o $@ $(LDFLAGS)

# assembly
$(BUILD_DIR)/%.s.o: %.s
	$(MKDIR_P) $(dir $@)
	$(AS) $(ASFLAGS) -c $< -o $@

# c source
$(BUILD_DIR)/%.c.o: %.c
	$(MKDIR_P) $(dir $@)
	clang-format --verbose -i -style=Google $<
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

.PHONY: clean

clean:
	$(RM) -r $(BUILD_DIR)

-include $(DEPS)

MKDIR_P ?= mkdir -p
