################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
Crosshairs/Commros_user_regular.obj: ../Crosshairs/Commros_user_regular.c $(GEN_OPTS) $(GEN_SRCS)
	@echo 'Building file: $<'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccsv5/tools/compiler/c2000_6.1.3/bin/cl2000" -v28 -ml -mt -O3 -g --include_path="C:/ti/ccsv5/tools/compiler/c2000_6.1.3/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/include" --include_path="C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/include" --include_path="C:/ti/controlSUITE/development_kits/~SupportFiles/F2802x_headers" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/asm" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/source/C28x_SGEN_LIB" --include_path="c:/ti/controlSUITE/libs/dsp/SGEN/v101/include" --include_path="C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/include" --include_path="C:/Users/Toby/git/F2802x BurnInUnit/F2802x_BurnInUnit/Crosshairs" --define="_DEBUG" --define="LARGE_MODEL" --define="FLASH" --diag_warning=225 --preproc_with_compile --preproc_dependency="Crosshairs/Commros_user_regular.pp" --obj_directory="Crosshairs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


