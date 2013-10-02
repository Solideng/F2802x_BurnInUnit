################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
ADC_SOC_Cnf.obj: C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/C/ADC_SOC_Cnf.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="ADC_SOC_Cnf.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Adc.obj: ../Adc.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Adc.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

BurnInUnit_ISR.obj: ../BurnInUnit_ISR.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="BurnInUnit_ISR.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Cntl.obj: ../Cntl.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Cntl.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DSP2802x_CodeStartBranch.obj: C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/source/DSP2802x_CodeStartBranch.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2802x_CodeStartBranch.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DSP2802x_GlobalVariableDefs.obj: C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/source/DSP2802x_GlobalVariableDefs.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2802x_GlobalVariableDefs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DSP2802x_usDelay.obj: C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/source/DSP2802x_usDelay.asm $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="DSP2802x_usDelay.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

DevInit_F2802x.obj: ../DevInit_F2802x.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="DevInit_F2802x.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

EnableCtrl.obj: ../EnableCtrl.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="EnableCtrl.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

I2c.obj: ../I2c.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="I2c.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

MacroNets.obj: ../MacroNets.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="MacroNets.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Main.obj: ../Main.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PWM_2ch_UpCnt_Cnf.obj: C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/C/PWM_2ch_UpCnt_Cnf.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="PWM_2ch_UpCnt_Cnf.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

PhaseCtrl.obj: ../PhaseCtrl.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="PhaseCtrl.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Pwm.obj: ../Pwm.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Pwm.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

SCPI_specificCmds.obj: ../SCPI_specificCmds.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="SCPI_specificCmds.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Sci.obj: ../Sci.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Sci.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

SineGen.obj: ../SineGen.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="SineGen.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

SlewControl.obj: ../SlewControl.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="SlewControl.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Spi.obj: ../Spi.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Spi.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

StateMachine.obj: ../StateMachine.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="StateMachine.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Timers.obj: ../Timers.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Timers.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

Tmp.obj: ../Tmp.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Tmp.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


