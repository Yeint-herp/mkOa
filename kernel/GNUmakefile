.SUFFIXES:
MAKEFLAGS += --no-builtin-rules --no-print-directory

MODE ?= debug

BUILD_DIR ?= $(abspath ../build)
KERNEL_DIR ?= $(abspath ../loader)

KERNEL_BUILD_DIR := $(BUILD_DIR)/kernel
OBJ_DIR := $(KERNEL_BUILD_DIR)/obj

KERNEL_SOURCE_DIR := $(KERNEL_DIR)/source

OUTPUT := $(BUILD_DIR)/krnl

CC := clang
LD := ld.lld

TARGET := x86_64-unknown-none-elf

LINKER_SCRIPT := linker.ld

CFLAGS := \
	--target=$(TARGET) \
	-I$(KERNEL_SOURCE_DIR) \
	-std=gnu23 \
	-ffreestanding \
	-fno-builtin \
	-fno-stack-protector \
	-fno-stack-check \
	-fno-lto \
	-fno-pic \
	-fno-pie \
	-ffunction-sections \
	-fdata-sections \
	-m64 \
	-march=x86-64 \
	-mabi=sysv \
	-mcmodel=kernel \
	-mno-red-zone \
	-mno-sse \
	-mno-sse2 \
	-mno-mmx \
	-mno-80387 \
	-Wall \
	-Wextra

LDFLAGS := \
	-m elf_x86_64 \
	-nostdlib \
	-static \
	-z max-page-size=0x1000 \
	--gc-sections \
	-T $(LINKER_SCRIPT)

ifeq ($(MODE),debug)
CFLAGS += -O0 -g -fno-omit-frame-pointer 
else
CFLAGS += -O2 -g0
endif

C_SRCS := $(shell find source -type f -name '*.c')
C_OBJS := $(patsubst source/%.c, $(OBJ_DIR)/%.o, $(C_SRCS))
C_DEPS := $(C_OBJS:.o=.d)

all: $(OUTPUT)

-include $(C_DEPS)

.PHONY: all clean

$(OUTPUT): $(C_OBJS) $(LINKER_SCRIPT)
	@mkdir -p $(dir $@)
	@echo "[LD] $@"
	@$(LD) $(LDFLAGS) -o $@ $(C_OBJS)

$(OBJ_DIR)/%.o: source/%.c
	@mkdir -p $(dir $@)
	@echo "[CC] $<"
	@$(CC) -MMD -MP $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(OBJ_DIR) $(OUTPUT)
