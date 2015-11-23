################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
SIM800C.obj: ../SIM800C.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --include_path="C:/Users/Rob/git/solarity_git/solarity/driverlib/MSP432P4xx" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="SIM800C.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

SL_Display.obj: ../SL_Display.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --include_path="C:/Users/Rob/git/solarity_git/solarity/driverlib/MSP432P4xx" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="SL_Display.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --include_path="C:/Users/Rob/git/solarity_git/solarity/driverlib/MSP432P4xx" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

msp432_startup_ccs.obj: ../msp432_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --include_path="C:/Users/Rob/git/solarity_git/solarity/driverlib/MSP432P4xx" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="msp432_startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

printf.obj: ../printf.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_15.9.0.STS/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Rob/git/solarity_git/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --include_path="C:/Users/Rob/git/solarity_git/solarity/driverlib/MSP432P4xx" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --display_error_number --diag_warning=225 --diag_wrap=off --preproc_with_compile --preproc_dependency="printf.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


