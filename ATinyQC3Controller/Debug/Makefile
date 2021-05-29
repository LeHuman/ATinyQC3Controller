################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS :=  \
../Mod \
../Mod/button \
../Mod/millis \
../Mod/conf


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../main.c \
../Mod/button/button.c \
../Mod/millis/millis.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
main.o \
Mod/button/button.o \
Mod/millis/millis.o

OBJS_AS_ARGS +=  \
main.o \
Mod/button/button.o \
Mod/millis/millis.o

C_DEPS +=  \
main.d \
Mod/button/button.d \
Mod/millis/millis.d

C_DEPS_AS_ARGS +=  \
main.d \
Mod/button/button.d \
Mod/millis/millis.d

OUTPUT_FILE_PATH +=ATinyQC3Controller.elf

OUTPUT_FILE_PATH_AS_ARGS +=ATinyQC3Controller.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+= 

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler
./main.o: .././main.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -Wextra -mmcu=attiny45 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\gcc\dev\attiny45" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Mod/button/button.o: ../Mod/button/button.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -Wextra -mmcu=attiny45 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\gcc\dev\attiny45" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	

Mod/millis/millis.o: ../Mod/millis/millis.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\include"  -Os -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -Wall -Wextra -mmcu=attiny45 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\gcc\dev\attiny45" -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	




# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 5.4.0
	$(QUOTE)C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="ATinyQC3Controller.map" -Wl,--start-group -Wl,-lm  -Wl,--end-group -Wl,--gc-sections -mmcu=attiny45 -B "C:\Program Files (x86)\Atmel\Studio\7.0\Packs\atmel\ATtiny_DFP\1.8.332\gcc\dev\attiny45"  
	@echo Finished building target: $@
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "ATinyQC3Controller.elf" "ATinyQC3Controller.hex"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "ATinyQC3Controller.elf" "ATinyQC3Controller.eep" || exit 0
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objdump.exe" -h -S "ATinyQC3Controller.elf" > "ATinyQC3Controller.lss"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "ATinyQC3Controller.elf" "ATinyQC3Controller.srec"
	"C:\Program Files (x86)\Atmel\Studio\7.0\toolchain\avr8\avr8-gnu-toolchain\bin\avr-size.exe" "ATinyQC3Controller.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "ATinyQC3Controller.elf" "ATinyQC3Controller.a" "ATinyQC3Controller.hex" "ATinyQC3Controller.lss" "ATinyQC3Controller.eep" "ATinyQC3Controller.map" "ATinyQC3Controller.srec" "ATinyQC3Controller.usersignatures"
	