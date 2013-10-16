################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/cmd/DSP2802x_Headers_nonBIOS.cmd 

CMD_UPPER_SRCS += \
../F2802x_BurnInUnit.CMD 

LIB_SRCS += \
C:/ti/controlSUITE/libs/math/IQmath/v15c/lib/IQmath.lib 

ASM_SRCS += \
../BurnInUnit_ISR.asm \
C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/source/DSP2802x_CodeStartBranch.asm \
C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/source/DSP2802x_usDelay.asm 

C_SRCS += \
C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/C/ADC_SOC_Cnf.c \
../Adc.c \
../Cntl.c \
../Comparator.c \
C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/source/DSP2802x_GlobalVariableDefs.c \
../DevInit_F2802x.c \
../EnableCtrl.c \
../I2c.c \
../MacroNets.c \
../Main.c \
../Ocp.c \
../Otp.c \
../Ovp.c \
C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/C/PWM_2ch_UpCnt_Cnf.c \
../PhaseCtrl.c \
../Pwm.c \
../SCPI_specificCmds.c \
../Sci.c \
../SineGen.c \
../SlewControl.c \
../Spi.c \
../StateMachine.c \
../Timers.c \
../Tmp.c \
../slaveMode.c 

OBJS += \
./ADC_SOC_Cnf.obj \
./Adc.obj \
./BurnInUnit_ISR.obj \
./Cntl.obj \
./Comparator.obj \
./DSP2802x_CodeStartBranch.obj \
./DSP2802x_GlobalVariableDefs.obj \
./DSP2802x_usDelay.obj \
./DevInit_F2802x.obj \
./EnableCtrl.obj \
./I2c.obj \
./MacroNets.obj \
./Main.obj \
./Ocp.obj \
./Otp.obj \
./Ovp.obj \
./PWM_2ch_UpCnt_Cnf.obj \
./PhaseCtrl.obj \
./Pwm.obj \
./SCPI_specificCmds.obj \
./Sci.obj \
./SineGen.obj \
./SlewControl.obj \
./Spi.obj \
./StateMachine.obj \
./Timers.obj \
./Tmp.obj \
./slaveMode.obj 

ASM_DEPS += \
./BurnInUnit_ISR.pp \
./DSP2802x_CodeStartBranch.pp \
./DSP2802x_usDelay.pp 

C_DEPS += \
./ADC_SOC_Cnf.pp \
./Adc.pp \
./Cntl.pp \
./Comparator.pp \
./DSP2802x_GlobalVariableDefs.pp \
./DevInit_F2802x.pp \
./EnableCtrl.pp \
./I2c.pp \
./MacroNets.pp \
./Main.pp \
./Ocp.pp \
./Otp.pp \
./Ovp.pp \
./PWM_2ch_UpCnt_Cnf.pp \
./PhaseCtrl.pp \
./Pwm.pp \
./SCPI_specificCmds.pp \
./Sci.pp \
./SineGen.pp \
./SlewControl.pp \
./Spi.pp \
./StateMachine.pp \
./Timers.pp \
./Tmp.pp \
./slaveMode.pp 

C_DEPS__QUOTED += \
"ADC_SOC_Cnf.pp" \
"Adc.pp" \
"Cntl.pp" \
"Comparator.pp" \
"DSP2802x_GlobalVariableDefs.pp" \
"DevInit_F2802x.pp" \
"EnableCtrl.pp" \
"I2c.pp" \
"MacroNets.pp" \
"Main.pp" \
"Ocp.pp" \
"Otp.pp" \
"Ovp.pp" \
"PWM_2ch_UpCnt_Cnf.pp" \
"PhaseCtrl.pp" \
"Pwm.pp" \
"SCPI_specificCmds.pp" \
"Sci.pp" \
"SineGen.pp" \
"SlewControl.pp" \
"Spi.pp" \
"StateMachine.pp" \
"Timers.pp" \
"Tmp.pp" \
"slaveMode.pp" 

OBJS__QUOTED += \
"ADC_SOC_Cnf.obj" \
"Adc.obj" \
"BurnInUnit_ISR.obj" \
"Cntl.obj" \
"Comparator.obj" \
"DSP2802x_CodeStartBranch.obj" \
"DSP2802x_GlobalVariableDefs.obj" \
"DSP2802x_usDelay.obj" \
"DevInit_F2802x.obj" \
"EnableCtrl.obj" \
"I2c.obj" \
"MacroNets.obj" \
"Main.obj" \
"Ocp.obj" \
"Otp.obj" \
"Ovp.obj" \
"PWM_2ch_UpCnt_Cnf.obj" \
"PhaseCtrl.obj" \
"Pwm.obj" \
"SCPI_specificCmds.obj" \
"Sci.obj" \
"SineGen.obj" \
"SlewControl.obj" \
"Spi.obj" \
"StateMachine.obj" \
"Timers.obj" \
"Tmp.obj" \
"slaveMode.obj" 

ASM_DEPS__QUOTED += \
"BurnInUnit_ISR.pp" \
"DSP2802x_CodeStartBranch.pp" \
"DSP2802x_usDelay.pp" 

C_SRCS__QUOTED += \
"C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/C/ADC_SOC_Cnf.c" \
"../Adc.c" \
"../Cntl.c" \
"../Comparator.c" \
"C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_headers/source/DSP2802x_GlobalVariableDefs.c" \
"../DevInit_F2802x.c" \
"../EnableCtrl.c" \
"../I2c.c" \
"../MacroNets.c" \
"../Main.c" \
"../Ocp.c" \
"../Otp.c" \
"../Ovp.c" \
"C:/ti/controlSUITE/libs/app_libs/digital_power/f2802x_v3.1/C/PWM_2ch_UpCnt_Cnf.c" \
"../PhaseCtrl.c" \
"../Pwm.c" \
"../SCPI_specificCmds.c" \
"../Sci.c" \
"../SineGen.c" \
"../SlewControl.c" \
"../Spi.c" \
"../StateMachine.c" \
"../Timers.c" \
"../Tmp.c" \
"../slaveMode.c" 

ASM_SRCS__QUOTED += \
"../BurnInUnit_ISR.asm" \
"C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/source/DSP2802x_CodeStartBranch.asm" \
"C:/ti/controlSUITE/device_support/f2802x/v128/DSP2802x_common/source/DSP2802x_usDelay.asm" 


