board_name := openbotbrain
PROJECT_DIR := $(CURDIR)

TIME_START := $(shell date +%s)

# ------------------------------- Arm Toolchain Path -------------------------------------
ifeq ($(OS),Windows_NT)
	TOOLCHAIN_ROOT:=C:/TOOLCHAIN/9-2019-q4-mayor
else
	TOOLCHAIN_ROOT:=/opt/arm-toolchain/gcc-arm-none-eabi-9-2019-q4-major
endif

# ----------------------------- Dependencies stuff ----------------------------------------
dependency_dir := $(PROJECT_DIR)/result/$(board_name)_dep
build_dir := $(PROJECT_DIR)/result/$(board_name)_build
bin_dir := $(PROJECT_DIR)/result/$(board_name)_binary

$(shell mkdir -p $(dependency_dir) >/dev/null)
$(shell mkdir -p $(build_dir) >/dev/null)
$(shell mkdir -p $(bin_dir) >/dev/null)

# --------------------------- GIT CONFIGURATION --------------------------------------------
# Git must be installed on pc to make this work!!!
GIT_HASH := $(shell git rev-parse --verify HEAD --short)
GIT_BRANCH := $(shell git rev-parse --abbrev-ref HEAD)
BUILD_DEVELOPER := $(shell git config user.name)
BUILD_VERSION := $(shell git describe --tags --long --dirty --always --match 'v[0-9]*\.[0-9]*')

ifeq ($(OS),Windows_NT)
	BUILD_DATE := $(shell cmd /C date /t)
else
	BUILD_DATE := $(shell date)
endif

# ------------------------------------------------------------------------------------------
# Global definition that can be used in code, values are taken from git.
GIT_DEF := -DBUILD_GIT_HASH=\"'$(GIT_HASH)'\"
GIT_DEF += -DBUILD_DATE=\"'$(BUILD_DATE)'\"
GIT_DEF += -DBUILD_DEVELOPER=\"'$(BUILD_DEVELOPER)'\"
GIT_DEF += -DBUILD_BRANCH=\"'$(GIT_BRANCH)'\"
GIT_DEF += -DBUILD_VERSION=\"'$(BUILD_VERSION)'\"

CC := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc
CXX := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-g++
LD := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gcc
OBJCOPY := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-objcopy
AS := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-as
AR := $(TOOLCHAIN_ROOT)/bin/arm-none-eabiar
GDB := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-gdb
SIZE := $(TOOLCHAIN_ROOT)/bin/arm-none-eabi-size --format=bekeley

# ------------------------------ Main Aplication --------------------------------------------
root_folder_path := $(PROJECT_DIR)/controller

application_path := $(root_folder_path)/app
config_path := $(root_folder_path)/config
startup_linker_path := $(root_folder_path)/startup-linker
stm_hal_library_path := $(root_folder_path)/stm-hal
library_path := $(PROJECT_DIR)/libs

# Linker File
linker_source_file := $(startup_linker_path)/STM32L496ZGTx_FLASH.ld

# StartUp
asm_source_path := $(startup_linker_path)/
asm_source_files := startup_stm32l496xx.s

# ------------------------------------------------------------------------------------------
board_include_paths := \
	-I$(root_folder_path)/ \
	-I$(application_path)/ \
	-I$(config_path)/ \
	-I$(stm_hal_library_path)/usb \

all_source_path := \
	$(application_path) \
	$(application_path)/system \
	$(application_path)/task \
	$(application_path)/module \
	$(config_path) \
	$(stm_hal_library_path) \
	$(stm_hal_library_path)/usb

# Main System File
CXXSRC:=main.cpp
CSRC:=assert.c

# System Folder
CXXSRC+=system-freertos.cpp
CXXSRC+=system-status.cpp
CXXSRC+=system-safe-i2c.cpp
CXXSRC+=system-version.cpp

# Task
CXXSRC+=task-blinky.cpp
CXXSRC+=task-display.cpp
CXXSRC+=task-gscope.cpp
CXXSRC+=task-imu.cpp
CXXSRC+=task-ports.cpp
CXXSRC+=task-power-supply.cpp

# Config Folder
CXXSRC+=board-rev.cpp
CXXSRC+=board-rev-a.cpp

# Module folder
CXXSRC+=module-input.cpp
CXXSRC+=module-output.cpp
CXXSRC+=module-ports.cpp

# ------------------------------------------------------------------------------------------
# stm-hal low level config files APP - Source and Include
all_source_path += $(stm_hal_libraty_path)

CXXSRC+=hal-adc.cpp
CXXSRC+=hal-board-init.cpp
CXXSRC+=hal-clock.cpp
CXXSRC+=hal-exti.cpp
CXXSRC+=hal-gpio.cpp
CXXSRC+=hal-i2c.cpp
CSRC+=hal-sys-irq.c
CSRC+=hal-sys-init.c
CXXSRC+=hal-spi.cpp
CXXSRC+=hal-tim.cpp
CXXSRC+=hal-uart.cpp
CXXSRC+=hal-usb.cpp

# STM-Hal - USB
CSRC+=usbd_conf.c
CSRC+=usbd_desc.c

# ------------------------------------------------------------------------------------------
# ST Driver Files - Source and Include
driver_include_paths := \
	-I$(library_path)/STM32L4xx_HAL_Driver/Inc/ \
	-I$(library_path)/STM32L4xx_HAL_Driver/Inc/Legacy/ \
	-I$(library_path)/CMSIS/Include/ \
	-I$(library_path)/CMSIS/Device/ST/STM32L4xx/Include/

board_include_paths += $(driver_include_paths)

all_source_path += \
	$(library_path)/CMSIS/ \
	$(library_path)/STM32L4xx_HAL_Driver/ \
	$(library_path)/STM32L4xx_HAL_Driver/Src/ \
	$(library_path)/STM32L4xx_HAL_Driver/Src/CMSIS_RTOS/ \

CSRC+=stm32l4xx_hal_adc.c
CSRC+=stm32l4xx_hal_adc_ex.c
CSRC+=stm32l4xx_hal.c
CSRC+=stm32l4xx_hal_i2c.c
CSRC+=stm32l4xx_hal_i2c_ex.c
CSRC+=stm32l4xx_hal_rcc.c
CSRC+=stm32l4xx_hal_rcc_ex.c
CSRC+=stm32l4xx_hal_flash.c
CSRC+=stm32l4xx_hal_flash_ex.c
CSRC+=stm32l4xx_hal_flash_ramfunc.c
CSRC+=stm32l4xx_hal_gpio.c
CSRC+=stm32l4xx_hal_dma.c
CSRC+=stm32l4xx_hal_dma_ex.c
CSRC+=stm32l4xx_hal_pwr.c
CSRC+=stm32l4xx_hal_pwr_ex.c
CSRC+=stm32l4xx_hal_cortex.c
CSRC+=stm32l4xx_hal_exti.c
CSRC+=stm32l4xx_hal_uart.c
CSRC+=stm32l4xx_hal_uart_ex.c
CSRC+=stm32l4xx_hal_spi.c
CSRC+=stm32l4xx_hal_spi_ex.c
CSRC+=stm32l4xx_hal_tim.c
CSRC+=stm32l4xx_hal_tim_ex.c
CSRC+=stm32l4xx_hal_pcd.c
CSRC+=stm32l4xx_hal_pcd_ex.c
CSRC+=stm32l4xx_ll_usb.c

# ------------------------------------------------------------------------------------------
# USB Files - Source and Include
usb_device_path := $(library_path)/STM32_USB_Device_Library
board_include_paths += \
	-I$(usb_device_path)/Core/Inc/ \
	-I$(usb_device_path)/Class/CDC/Inc/

all_source_path += $(usb_device_path)/Core/Src/ \
                   $(usb_device_path)/Class/CDC/Src/ \
# Individual files
CSRC+=usbd_core.c
CSRC+=usbd_ctlreq.c
CSRC+=usbd_ioreq.c
CSRC+=usbd_cdc.c

# ------------------------------------------------------------------------------------------
# FreeRTOS Files - Source and Include
os_path := $(library_path)/FreeRTOS

os_include_paths := \
	-I$(os_path)/Source/include \
	-I$(os_path)/Source/portable/GCC/ARM_CM4F/ \
	-I$(os_path)/Source/CMSIS_RTOS_V2/

board_include_paths += $(os_include_paths)

all_source_path += \
	$(os_path)/ \
	$(os_path)/Source/ \
	$(os_path)/Source/portable/MemMang/ \
	$(os_path)/Source/portable/GCC/ARM_CM4F/ \
	$(os_path)/Source/CMSIS_RTOS_V2/ \

# Individual files
CSRC+=croutine.c
CSRC+=event_groups.c
CSRC+=list.c
CSRC+=queue.c
CSRC+=tasks.c
CSRC+=timers.c
CSRC+=port.c

# -------------------------------------------------------------------------
# Library - Submodule Files - Source and Include
gscope_path := $(library_path)/gscope
board_include_paths += -I$(gscope_path)/include/
custom_libs := -L$(gscope_path) -lgscope_mcu_cortex_m4_fpv4

board_include_paths += -I$(library_path)/serial_reception/include/

# ------------------------------------------------------------------------------------------
# Library - Submodule Files - Source and Include
bq25601_path := $(library_path)/bq25601
board_include_paths += -I$(bq25601_path)/include/
all_source_path += $(bq25601_path)/src

CXXSRC+=bq25601.cpp

# Library - Submodule Files - Source and Include
lsm6ds_path := $(library_path)/lsm6dsl
board_include_paths += -I$(lsm6ds_path)/include/
all_source_path += $(lsm6ds_path)/src

CXXSRC+=lsm6dsl.cpp

# ------------------------------------------------------------------------------------------
# Library - Submodule Files - Source and Include
all_source_path += $(library_path)/ssd1306/src
board_include_paths += -I$(library_path)/ssd1306/include/

CXXSRC+=ssd1306.cpp
CXXSRC+=glcdfont.cpp
CXXSRC+=gscommon-graph.cpp

# ------------------------------------------------------------------------------------------
# Library - Submodule Files - Source and Include
all_source_path += $(library_path)/gsmenu/src
board_include_paths += -I$(library_path)/gsmenu/include/

CXXSRC+=gsmenu.cpp

# --------------------------- BUILD FLAG CONFIGURATION -------------------------------------
DEBUG_OPTLVL := 0	# Optimization level, can be [0, 1, 2, 3, s].
DBG:=-g

cdefs :=-DUSE_HAL_DRIVER -DSTM32L496xx
float_options := -mfpu=fpv4-sp-d16 -mfloat-abi=hard
cpu_type = -mcpu=cortex-m4
MCU := $(cpu_type) -mthumb $(float_options)

common_flags :=  $(DBG) $(GIT_DEF) $(MCU) -Wall -Wunused-function -Wno-write-strings -Werror
common_flags += -Wunused-but-set-variable -Wunused-variable -Wunused-parameter
common_flags += -fstack-usage -Wstack-usage=512 -fno-exceptions -Wno-psabi -Os
common_flags += -fno-strict-aliasing -fdata-sections -ffunction-sections -specs=nano.specs -specs=nosys.specs
common_flags += -O$(DEBUG_OPTLVL)

assembler_options := -O$(REGULAR_PTLVL)
c_compiler_options := $(cdefs) $(common_flags) $(board_include_paths)
cpp_compiler_options := $(cdefs) $(common_flags) $(board_include_paths) -fno-rtti -std=c++17 -fno-threadsafe-statics
linker_options := -Wl,-static $(common_flags) -Wl,--gc-sections,-T$(linker_source_file) -Wl,--print-memory-usage
dependency_options = -MT $@ -MMD -MP -MF $(dependency_dir)/$*.Td
ld_lib := -lc -lnosys -lm $(custom_libs)

# ------------------------------------------------------------------------------------------
TIME_DIFF_MS := $$(($$(date +%s) - $(TIME_START)))

# ------------------------------------------------------------------------------------------
# Hack to always compile git information file. Touch change the file timestamp
HACK:=$(shell touch $(application_path)/system/system-version.cpp)

# vpath is used so object files are written to the current directory instead
# of the same directory as their source files
vpath %.c $(all_source_path)
vpath %.cpp $(all_source_path)
vpath %.s $(asm_source_path)

COBJ = $(CSRC:%.c=$(build_dir)/%.o)
CXXOBJ = $(CXXSRC:%.cpp=$(build_dir)/%.o)
OBJ = $(COBJ) $(CXXOBJ)

COMPILE_CC = $(CCACHE) $(CC) $(dependency_options) $(c_compiler_options)
COMPILE_CXX = $(CCACHE) $(CXX) $(dependency_options) $(cpp_compiler_options)

POSTCOMPILE = mv -f $(dependency_dir)/$*.Td $(dependency_dir)/$*.d

$(build_dir)/%.o: %.cpp
	@echo [CXX] $(notdir $<)
	@$(COMPILE_CXX) $< -c -o $@
	@$(POSTCOMPILE)

$(build_dir)/%.o: %.c
	@echo [CC] $(notdir $<)
	@$(COMPILE_CC) $< -c -o $@
	@$(POSTCOMPILE)

$(dependency_dir)/%.d: ;
.PRECIOUS: $(dependency_dir)/%.d

all: $(OBJ)
	@echo [AS] $(asm_source_files)
	@$(AS) -o $(asm_source_files:%.s=$(build_dir)/%.o) $(asm_source_path)/$(asm_source_files)
	@echo "-------------------------------------------------------------------------------------"
	@echo "[INFO] Linking objects into: $(board_name).elf"

	@$(CC) -o $(bin_dir)/$(board_name).elf $(OBJ) $(asm_source_files:%.s=$(build_dir)/%.o) \
		$(ld_lib) -Wl,-Map=$(bin_dir)/$(board_name).map $(linker_options) $(linker_libs)

	@echo "[INFO] Generating application binary: $(board_name).hex and $(board_name).bin"
	@$(OBJCOPY) -O ihex $(bin_dir)/$(board_name).elf $(bin_dir)/$(board_name).hex
	@$(OBJCOPY) -O binary $(bin_dir)/$(board_name).elf $(bin_dir)/$(board_name).bin

	@echo "-------------------------------------------------------------------------------------"
	@echo "[SIZE] Total binary size:"
	@$(SIZE) $(bin_dir)/$(board_name).elf

	@echo "-------------------------------------------------------------------------------------"
	@echo "Build Success by  $(BUILD_DEVELOPER) - $(BUILD_DATE)"
	@echo "[Git Hash]: $(GIT_HASH) [Git Branch]:$(GIT_BRANCH) [Build Version]: $(BUILD_VERSION)"
	@echo "Build took: $(TIME_DIFF_MS) seconds"
	@echo "-------------------------------------------------------------------------------------"

-include $(patsubst %,$(dependency_dir)/%.d,$(basename $(CSRC) $(CXXSRC)))

clean:
	@echo [RM] OBJ
	@rm -rf $(build_dir)
	@echo [RM] BIN
	@rm -rf $(bin_dir)/
	@echo [RM] DEPENDENCIES
	@rm -rf $(dependency_dir)/