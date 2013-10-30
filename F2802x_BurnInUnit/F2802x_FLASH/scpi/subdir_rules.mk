################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
scpi/SCPI_buildTree.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_buildTree.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_buildTree.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_commonCmds.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_commonCmds.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_commonCmds.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_execCtrl.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_execCtrl.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_execCtrl.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_inputBuffer.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_inputBuffer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_inputBuffer.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_lexer.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_lexer.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_lexer.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_msgEPCtrl.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_msgEPCtrl.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_msgEPCtrl.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_outputQueue.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_outputQueue.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_outputQueue.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_parser.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_parser.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_parser.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_requiredCmds.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_requiredCmds.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_requiredCmds.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_respFmtr.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_respFmtr.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_respFmtr.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_serialEmulation.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_serialEmulation.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_serialEmulation.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_status.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_status.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_status.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

scpi/SCPI_string.obj: C:/Users/Toby/git/SCPI_Build/SCPI_Build/scpi/SCPI_string.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="scpi/SCPI_string.pp" --obj_directory="scpi" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


