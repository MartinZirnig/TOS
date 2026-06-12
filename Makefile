CC = gcc
ASM = nasm
LD = ld

BUILD = build
DISK = $(BUILD)/os_disk.img

INCLUDE_DIRS = \
    src/shared \
    src/shared/HW_IO \
    src/shared/String \
    src/shared/Memory \
    src/shared/VGA \
    src/shared/Paging \
    src/shared/Math \
    src/shared/Context

INCLUDES = $(addprefix -I,$(INCLUDE_DIRS))

# ---------------------------------------------------------------------
# SPECIFICKÉ PŘÍZNAKY PRO 32-BIT (BOOTSTRAP) A 64-BIT (KERNEL)
# ---------------------------------------------------------------------

# Společný základ pro C kompilátor (freestanding OS vývoj)
CFLAGS_BASE = -ffreestanding -nostdinc -fno-pie -fno-stack-protector -nostdlib $(INCLUDES)

# 32-bit (Bootstrap) flagy
CFLAGS_32  = -m32 $(CFLAGS_BASE)
ASMFLAGS_32 = -f elf32
LDFLAGS_32  = -m elf_i386 --no-warn-rwx-segments --oformat binary

# 64-bit (Kernel) flagy
CFLAGS_64  = -m64 -mcmodel=kernel -mno-red-zone $(CFLAGS_BASE)
ASMFLAGS_64 = -f elf64
LDFLAGS_64  = -m elf_x86_64 --no-warn-rwx-segments --oformat binary


# ---------------------------------------------------------------------
# DETEKCE ZDROJÁKŮ A EXPLICITNÍ SEPARACE OBJEKTŮ SE SUFFIXY
# ---------------------------------------------------------------------
C_SOURCES   = $(shell find src -name "*.c")
ASM_SOURCES = $(shell find src -name "*.asm")

# Objekty Bootloaderu (pouze hlavní soubor, dostane __asm)
BOOTLOADER_OBJS = $(BUILD)/bootloader/bootloader__asm.o

# Objekty Bootstrapu (vše v src/bootstrap/ kompilujeme jako 32-bit)
BOOTSTRAP_C_SRCS   = $(shell find src/bootstrap -name "*.c")
BOOTSTRAP_ASM_SRCS = $(shell find src/bootstrap -name "*.asm")

# Vstupní bod je teď bootstrap__asm.o
BOOTSTRAP_ENTRY    = $(BUILD)/bootstrap/bootstrap__asm.o

# Generování objektů se suffixy __c a __asm
BOOTSTRAP_REST     = $(patsubst src/%.c,$(BUILD)/%__c.o,$(BOOTSTRAP_C_SRCS)) \
                     $(patsubst src/%.asm,$(BUILD)/%__asm.o,$(BOOTSTRAP_ASM_SRCS))

# Složíme objekty tak, aby bootstrap__asm.o (vstupní bod) byl VŽDY na prvním místě!
BOOTSTRAP_OBJS     = $(BOOTSTRAP_ENTRY) $(filter-out $(BOOTSTRAP_ENTRY),$(BOOTSTRAP_REST))

# Objekty Kernelu (vše v src/kernel/ kompilujeme jako 64-bit)
KERNEL_C_SRCS   = $(shell find src/kernel -name "*.c")
KERNEL_ASM_SRCS = $(shell find src/kernel -name "*.asm")
KERNEL_OBJS     = $(patsubst src/%.c,$(BUILD)/%__c.o,$(KERNEL_C_SRCS)) \
                  $(patsubst src/%.asm,$(BUILD)/%__asm.o,$(KERNEL_ASM_SRCS))

# Objekty Shared (Adresář src/shared/ se zkompiluje 2x - jednou pro 32b, jednou pro 64b)
SHARED_C_SRCS   = $(shell find src/shared -name "*.c")
SHARED_ASM_SRCS = $(shell find src/shared -name "*.asm")

SHARED_OBJS_32  = $(patsubst src/%.c,$(BUILD)/%__c_32.o,$(SHARED_C_SRCS)) \
                  $(patsubst src/%.asm,$(BUILD)/%__asm_32.o,$(SHARED_ASM_SRCS))

SHARED_OBJS_64  = $(patsubst src/%.c,$(BUILD)/%__c_64.o,$(SHARED_C_SRCS)) \
                  $(patsubst src/%.asm,$(BUILD)/%__asm_64.o,$(SHARED_ASM_SRCS))


.PHONY: all clean run env-init env-fill docker-build docker-run build run-kill

# Hlavní cíl sestaví všechny 3 binárky
all: $(BUILD)/bootloader.bin $(BUILD)/bootstrap.bin $(BUILD)/kernel.bin

# ---------------------------------------------------------------------
# KOMPILACE PRO BOOTLOADER
# ---------------------------------------------------------------------
$(BUILD)/bootloader/%__asm.o: src/bootloader/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) -f elf32 $< -o $@

# ---------------------------------------------------------------------
# KOMPILACE PRO BOOTSTRAP (Čistý 32-bit s rozlišením suffixů)
# ---------------------------------------------------------------------
$(BUILD)/bootstrap/%__c.o: src/bootstrap/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_32) -c $< -o $@

$(BUILD)/bootstrap/%__asm.o: src/bootstrap/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_32) $< -o $@

# Kompilace sdílených souborů pro 32-bit bootstrap
$(BUILD)/shared/%__c_32.o: src/shared/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_32) -c $< -o $@

$(BUILD)/shared/%__asm_32.o: src/shared/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_32) $< -o $@

# ---------------------------------------------------------------------
# KOMPILACE PRO KERNEL (Čistý 64-bit s rozlišením suffixů)
# ---------------------------------------------------------------------
$(BUILD)/kernel/%__c.o: src/kernel/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_64) -c $< -o $@

$(BUILD)/kernel/%__asm.o: src/kernel/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_64) $< -o $@

# Kompilace sdílených souborů pro 64-bit kernel
$(BUILD)/shared/%__c_64.o: src/shared/%.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS_64) -c $< -o $@

$(BUILD)/shared/%__asm_64.o: src/shared/%.asm
	@mkdir -p $(dir $@)
	@$(ASM) $(ASMFLAGS_64) $< -o $@


# ---------------------------------------------------------------------
# PRAVIDLA PRO LINKOVÁNÍ
# ---------------------------------------------------------------------

# 1) STAGE 1: Bootloader
$(BUILD)/bootloader.bin: $(BOOTLOADER_OBJS)
	@mkdir -p $(BUILD)
	@echo ""
	@echo "=================================================="
	@echo "   LINKING STAGE 1: Bootloader (0x7C00) [16-bit]"
	@echo "=================================================="
	@$(LD) $(LDFLAGS_32) -T linker/bootloader.ld -o $@ $^

# 2) STAGE 2: Bootstrap (Linkujeme 32-bitové objekty + 32-bit shared)
$(BUILD)/bootstrap.bin: $(BOOTSTRAP_OBJS) $(SHARED_OBJS_32)
	@mkdir -p $(BUILD)
	@echo ""
	@echo "=================================================="
	@echo "   LINKING STAGE 2: Bootstrap (0x8000) [32-bit]"
	@echo "=================================================="
	@$(LD) $(LDFLAGS_32) -T linker/bootstrap.ld -o $@ $^

# 3) STAGE 3: Kernel (Linkujeme 64-bitové objekty + 64-bit shared)
$(BUILD)/kernel.bin: $(KERNEL_OBJS) $(SHARED_OBJS_64)
	@mkdir -p $(BUILD)
	@echo ""
	@echo "=================================================="
	@echo "   LINKING STAGE 3: Kernel (0x0) [64-bit]"
	@echo "=================================================="
	@$(LD) $(LDFLAGS_64) -T linker/kernel.ld -o $@ $^


# ---------------------------------------------------------------------
# Disk Environment Management & QEMU
# ---------------------------------------------------------------------

env-init:
	@mkdir -p $(BUILD)
	@rm -f $(DISK)
	@qemu-img create -f raw $(DISK) 10G
	@echo "Disk image initialized: $(DISK) (10GB)"

env-fill: all
	@if [ ! -f $(DISK) ]; then echo "Error: Disk image does not exist. Run 'make env-init' first."; exit 1; fi
	@echo ""
	@echo "=================================================="
	@echo "   FLASHING BINARIES TO DISK IMAGE"
	@echo "=================================================="
	@echo "-> Writing Bootloader to sector 0..."
	@dd if=$(BUILD)/bootloader.bin of=$(DISK) bs=512 count=1 conv=notrunc 2>/dev/null
	@echo "-> Writing Bootstrap to sector 1..."
	@dd if=$(BUILD)/bootstrap.bin of=$(DISK) bs=512 seek=1 conv=notrunc 2>/dev/null
	@echo "-> Writing Kernel to offset 1GB..."
	@dd if=$(BUILD)/kernel.bin of=$(DISK) bs=1M seek=1024 conv=notrunc 2>/dev/null
	@echo "Done! Disk image is ready to boot."

run:
	@echo "Launching QEMU..."
	@qemu-system-x86_64 \
			-drive format=raw,file=$(DISK) \
			-m 4G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-d int,cpu_reset \
			-D qemu.log \
			-no-reboot \
			-no-shutdown

run-kill:
	@echo "Launching QEMU..."
	@qemu-system-x86_64 \
			-drive format=raw,file=$(DISK) \
			-m 4G \
			-device isa-debug-exit,iobase=0xf4,iosize=0x04 \
			-d int,cpu_reset \
			-D qemu.log \
			-no-reboot
			
	@echo Exit code: $?.

clean:
	@echo "Cleaning up build directory..."
	@rm -rf $(BUILD)
	@rm qemu.log -rf

docker-build:
	@docker build -t my-os-dev docker/

docker-run:
	@docker run -it --user $(shell id -u):$(shell id -g) -v $(PWD):/os my-os-dev

build:
	@clear
	@make clean
	@make all
	@make env-init
	@make env-fill