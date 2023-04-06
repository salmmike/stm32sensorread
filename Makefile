# path to STM32F103 standard peripheral library
STD_PERIPH_LIBS ?= ./STM32F10x_StdPeriph_Lib_V3.6.0

# list of source files
SRC = src
SOURCES  = ${SRC}/main.c ${SRC}/mpsgpio.c ${SRC}/mpsdht11.c ${SRC}/mpsdelay.c ${SRC}/mpsuart.c ${SRC}/mpslcd.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/system_stm32f10x.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/src/*.c
SOURCES += $(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/startup/TrueSTUDIO/startup_stm32f10x_md.s
BUILDDIR = build

PROJECT ?= sensorread

CC = arm-none-eabi-gcc
OBJCOPY = arm-none-eabi-objcopy

ST_FLASH ?= st-flash

# specify compiler flags
CFLAGS  = -g -O2 -Wall
CFLAGS += -T$(STD_PERIPH_LIBS)/Project/STM32F10x_StdPeriph_Template/TrueSTUDIO/STM3210B-EVAL/stm32_flash.ld
CFLAGS += -mlittle-endian -mthumb -mcpu=cortex-m4 -mthumb-interwork
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
CFLAGS += -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER
CFLAGS += -Wl,--gc-sections
CFLAGS += -I ./include/
CFLAGS += -I.
CFLAGS += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/DeviceSupport/ST/STM32F10x/
CFLAGS += -I$(STD_PERIPH_LIBS)/Libraries/CMSIS/CM3/CoreSupport
CFLAGS += -I$(STD_PERIPH_LIBS)/Libraries/STM32F10x_StdPeriph_Driver/inc

OBJS = $(SOURCES:.c=.o)

all: $(PROJECT).elf

# compile
$(PROJECT).elf: $(SOURCES)
	mkdir -p ${BUILDDIR}
	$(CC) $(CFLAGS) $^ -o ${BUILDDIR}/$@
	$(OBJCOPY) -O ihex ${BUILDDIR}/$(PROJECT).elf ${BUILDDIR}/$(PROJECT).hex
	$(OBJCOPY) -O binary ${BUILDDIR}/$(PROJECT).elf ${BUILDDIR}/$(PROJECT).bin

# remove binary files
clean:
	rm -f ${BUILDDIR}/*

# flash
flash:
	$(ST_FLASH) --reset write ${BUILDDIR}/$(PROJECT).bin 0x8000000
