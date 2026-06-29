.SUFFIXES:
MAKEFLAGS += --no-builtin-rules --no-builtin-variables --no-print-directory
SHELL := /bin/sh
.SHELLFLAGS := -eu -c

export ROOT := $(abspath .)
export PROJECT_NAME := Yerp

BUILD_DIR ?= $(abspath build)

MODE ?= debug

all: kernel hdd run

.PHONY: all kernel hdd run clean

KERNEL_DIR ?= $(abspath kernel)

kernel:
	@$(MAKE) -C $(KERNEL_DIR) \
		BUILD_DIR=$(BUILD_DIR) KERNEL_DIR=$(KERNEL_DIR) \
		MODE=$(MODE)

ESP_DIR := $(BUILD_DIR)/esp

hdd: kernel $(BUILD_DIR)/limine/limine
	@rm -rf $(ESP_DIR)
	@mkdir -p $(ESP_DIR)/EFI/BOOT

	@cp $(BUILD_DIR)/limine/BOOTX64.EFI $(ESP_DIR)/EFI/BOOT/BOOTX64.EFI
	@cp $(BUILD_DIR)/krnl $(ESP_DIR)/krnl
	@cp $(ROOT)/limine.conf $(ESP_DIR)/limine.conf

	@echo "[ESP] $(ESP_DIR)"

LIMINE_REPO := https://github.com/limine-bootloader/limine.git

LIMINE_BRANCH := v8.x-binary

$(BUILD_DIR)/limine/limine:
	@if [ ! -d "$(BUILD_DIR)/limine/limine" ]; then \
		git clone --branch $(LIMINE_BRANCH) --depth 1 $(LIMINE_REPO) $(BUILD_DIR)/limine; \
	fi
	@$(MAKE) -C $(BUILD_DIR)/limine

OVMF_CODE := $(BUILD_DIR)/OVMF_CODE.fd
OVMF_VARS := $(BUILD_DIR)/OVMF_VARS.fd

QEMUFLAGS := \
	-M q35 -cpu max,+x2apic -M accel=tcg,smm=off \
	-m 2G -smp 4 \
	-display none \
	-serial mon:stdio \
	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_CODE) \
	-drive if=pflash,format=raw,readonly=on,file=$(OVMF_VARS) \
	-no-reboot \
	-d int,cpu_reset -D $(BUILD_DIR)/qemu_log.txt \
	-s

run: $(OVMF_CODE) hdd
	@qemu-system-x86_64 \
		$(QEMUFLAGS) \
		-drive file=fat:rw:$(ESP_DIR),format=raw,if=none,id=nvme0,aio=threads,cache=unsafe \
		-device nvme,drive=nvme0,serial=deadbeef

$(OVMF_CODE):
	@curl -Lo $(OVMF_CODE) https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF_CODE.fd
	@curl -Lo $(OVMF_VARS) https://retrage.github.io/edk2-nightly/bin/RELEASEX64_OVMF_VARS.fd

clean:
	@$(MAKE) -C $(KERNEL_DIR) clean
	@rm -rf $(ESP_DIR) 
