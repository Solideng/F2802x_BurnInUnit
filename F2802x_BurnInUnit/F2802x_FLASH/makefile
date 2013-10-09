################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

CG_TOOL_ROOT := C:/ti/ccsv5/tools/compiler/c2000_6.1.3

ORDERED_OBJS += \
$(GEN_CMDS__FLAG) \
"./scpi/SCPI_string.obj" \
"./scpi/SCPI_status.obj" \
"./scpi/SCPI_serialEmulation.obj" \
"./scpi/SCPI_respFmtr.obj" \
"./scpi/SCPI_requiredCmds.obj" \
"./scpi/SCPI_parser.obj" \
"./scpi/SCPI_outputQueue.obj" \
"./scpi/SCPI_msgEPCtrl.obj" \
"./scpi/SCPI_lexer.obj" \
"./scpi/SCPI_inputBuffer.obj" \
"./scpi/SCPI_execCtrl.obj" \
"./scpi/SCPI_commonCmds.obj" \
"./scpi/SCPI_buildTree.obj" \
"./slaveMode.obj" \
"./Tmp.obj" \
"./Timers.obj" \
"./StateMachine.obj" \
"./Spi.obj" \
"./SlewControl.obj" \
"./SineGen.obj" \
"./Sci.obj" \
"./SCPI_specificCmds.obj" \
"./Pwm.obj" \
"./PhaseCtrl.obj" \
"./PWM_2ch_UpCnt_Cnf.obj" \
"./Main.obj" \
"./MacroNets.obj" \
"./I2c.obj" \
"./EnableCtrl.obj" \
"./DevInit_F2802x.obj" \
"./DSP2802x_usDelay.obj" \
"./DSP2802x_GlobalVariableDefs.obj" \
"./DSP2802x_CodeStartBranch.obj" \
"./Cntl.obj" \
"./BurnInUnit_ISR.obj" \
"./Adc.obj" \
"./ADC_SOC_Cnf.obj" \
-l"C:\ti\controlSUITE\libs\dsp\SGEN\v101\lib\C28x_SGEN_Lib_fixed.lib" \
-l"libc.a" \
"C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/cmd/DSP2802x_Headers_nonBIOS.cmd" \
"../F2802x_BurnInUnit.CMD" \
"C:/ti/controlSUITE/libs/math/IQmath/v15c/lib/IQmath.lib" \

-include ../makefile.init

RM := DEL /F
RMDIR := RMDIR /S/Q

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir_vars.mk
-include scpi/subdir_vars.mk
-include subdir_rules.mk
-include scpi/subdir_rules.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(S62_DEPS)),)
-include $(S62_DEPS)
endif
ifneq ($(strip $(C64_DEPS)),)
-include $(C64_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(S55_DEPS)),)
-include $(S55_DEPS)
endif
ifneq ($(strip $(C67_DEPS)),)
-include $(C67_DEPS)
endif
ifneq ($(strip $(C??_DEPS)),)
-include $(C??_DEPS)
endif
ifneq ($(strip $(CLA_DEPS)),)
-include $(CLA_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(S??_DEPS)),)
-include $(S??_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(C62_DEPS)),)
-include $(C62_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(K_DEPS)),)
-include $(K_DEPS)
endif
ifneq ($(strip $(C43_DEPS)),)
-include $(C43_DEPS)
endif
ifneq ($(strip $(S67_DEPS)),)
-include $(S67_DEPS)
endif
ifneq ($(strip $(SA_DEPS)),)
-include $(SA_DEPS)
endif
ifneq ($(strip $(S43_DEPS)),)
-include $(S43_DEPS)
endif
ifneq ($(strip $(OPT_DEPS)),)
-include $(OPT_DEPS)
endif
ifneq ($(strip $(S64_DEPS)),)
-include $(S64_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
ifneq ($(strip $(C55_DEPS)),)
-include $(C55_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: F2802x_BurnInUnit.out

# Tool invocations
F2802x_BurnInUnit.out: $(OBJS) $(CMD_SRCS) $(CMD_UPPER_SRCS) $(LIB_SRCS) $(GEN_CMDS)
	@echo 'Building target: $@'
	@echo 'Invoking: C2000 Linker'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 -z --stack_size=0x380 -m"F2802x_BurnInUnit.map" --warn_sections -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/lib" -i"{SGEN_LIB_VER_ROOT}/lib" -i"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" -i"C:/Users/Toby/git/F2802x_BurnInUnit/F2802x_BurnInUnit" --reread_libs --diag_suppress=--diag_suppress=16002 --xml_link_info="F2802x_BurnInUnit_linkInfo.xml" --rom_model -o "F2802x_BurnInUnit.out" $(ORDERED_OBJS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C2000_EXECUTABLE_OUTPUTS__QUOTED) "F2802x_BurnInUnit.out"
	-$(RM) "ADC_SOC_Cnf.pp" "Adc.pp" "Cntl.pp" "DSP2802x_GlobalVariableDefs.pp" "DevInit_F2802x.pp" "EnableCtrl.pp" "I2c.pp" "MacroNets.pp" "Main.pp" "PWM_2ch_UpCnt_Cnf.pp" "PhaseCtrl.pp" "Pwm.pp" "SCPI_specificCmds.pp" "Sci.pp" "SineGen.pp" "SlewControl.pp" "Spi.pp" "StateMachine.pp" "Timers.pp" "Tmp.pp" "slaveMode.pp" "scpi\SCPI_buildTree.pp" "scpi\SCPI_commonCmds.pp" "scpi\SCPI_execCtrl.pp" "scpi\SCPI_inputBuffer.pp" "scpi\SCPI_lexer.pp" "scpi\SCPI_msgEPCtrl.pp" "scpi\SCPI_outputQueue.pp" "scpi\SCPI_parser.pp" "scpi\SCPI_requiredCmds.pp" "scpi\SCPI_respFmtr.pp" "scpi\SCPI_serialEmulation.pp" "scpi\SCPI_status.pp" "scpi\SCPI_string.pp" 
	-$(RM) "ADC_SOC_Cnf.obj" "Adc.obj" "BurnInUnit_ISR.obj" "Cntl.obj" "DSP2802x_CodeStartBranch.obj" "DSP2802x_GlobalVariableDefs.obj" "DSP2802x_usDelay.obj" "DevInit_F2802x.obj" "EnableCtrl.obj" "I2c.obj" "MacroNets.obj" "Main.obj" "PWM_2ch_UpCnt_Cnf.obj" "PhaseCtrl.obj" "Pwm.obj" "SCPI_specificCmds.obj" "Sci.obj" "SineGen.obj" "SlewControl.obj" "Spi.obj" "StateMachine.obj" "Timers.obj" "Tmp.obj" "slaveMode.obj" "scpi\SCPI_buildTree.obj" "scpi\SCPI_commonCmds.obj" "scpi\SCPI_execCtrl.obj" "scpi\SCPI_inputBuffer.obj" "scpi\SCPI_lexer.obj" "scpi\SCPI_msgEPCtrl.obj" "scpi\SCPI_outputQueue.obj" "scpi\SCPI_parser.obj" "scpi\SCPI_requiredCmds.obj" "scpi\SCPI_respFmtr.obj" "scpi\SCPI_serialEmulation.obj" "scpi\SCPI_status.obj" "scpi\SCPI_string.obj" 
	-$(RM) "BurnInUnit_ISR.pp" "DSP2802x_CodeStartBranch.pp" "DSP2802x_usDelay.pp" 
	-@echo 'Finished clean'
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
