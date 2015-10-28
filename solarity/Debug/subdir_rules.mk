################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
SL_Display.obj: ../SL_Display.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Zach/workspace_v6_1/solarity/driverlib/MSP432P4xx" --include_path="C:/Users/Zach/workspace_v6_1/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Zach/workspace_v6_1/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="SL_Display.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

main.obj: ../main.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Zach/workspace_v6_1/solarity/driverlib/MSP432P4xx" --include_path="C:/Users/Zach/workspace_v6_1/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Zach/workspace_v6_1/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="main.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '

msp432_startup_ccs.obj: ../msp432_startup_ccs.c $(GEN_OPTS) $(GEN_HDRS)
	@echo 'Building file: $<'
	@echo 'Invoking: MSP432 Compiler'
	"C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 --abi=eabi -me --include_path="C:/ti/ccsv6/ccs_base/arm/include" --include_path="C:/Users/Zach/workspace_v6_1/solarity/driverlib/MSP432P4xx" --include_path="C:/Users/Zach/workspace_v6_1/solarity" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include" --include_path="C:/ti/ccsv6/ccs_base/arm/include/" --include_path="C:/ti/ccsv6/tools/compiler/ti-cgt-arm_5.2.5/include/" --include_path="C:/ti/ccsv6/ccs_base/arm/include/CMSIS" --include_path="C:/Users/Zach/workspace_v6_1/solarity" --include_path="C:/Users/Zach/Desktop/MSPware/MSPWare_2_30_00_49/driverlib/driverlib/MSP432P4xx/" --advice:power="all" -g --gcc --define=__MSP432P401R__ --define=TARGET_IS_MSP432P4XX --define=ccs --diag_wrap=off --diag_warning=225 --display_error_number --preproc_with_compile --preproc_dependency="msp432_startup_ccs.pp" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: $<'
	@echo ' '


