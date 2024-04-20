################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
CAN\ Devices/bms1000m.obj: ../CAN\ Devices/bms1000m.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=5 --fp_mode=strict --include_path="/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/include" --include_path="/Users/alexhernandez/workspace_june_23/VCU1200_Testbench" --include_path="C:/TI/TivaWare_C_Series-2.1.4.178" --define=ccs="ccs" --define=PART_TM4C123BH6PGE -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CAN Devices/bms1000m.d_raw" --obj_directory="CAN Devices" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CAN\ Devices/rms_inverter.obj: ../CAN\ Devices/rms_inverter.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=5 --fp_mode=strict --include_path="/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/include" --include_path="/Users/alexhernandez/workspace_june_23/VCU1200_Testbench" --include_path="C:/TI/TivaWare_C_Series-2.1.4.178" --define=ccs="ccs" --define=PART_TM4C123BH6PGE -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CAN Devices/rms_inverter.d_raw" --obj_directory="CAN Devices" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

CAN\ Devices/sme_acx1_inverter.obj: ../CAN\ Devices/sme_acx1_inverter.cpp $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=5 --fp_mode=strict --include_path="/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/include" --include_path="/Users/alexhernandez/workspace_june_23/VCU1200_Testbench" --include_path="C:/TI/TivaWare_C_Series-2.1.4.178" --define=ccs="ccs" --define=PART_TM4C123BH6PGE -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="CAN Devices/sme_acx1_inverter.d_raw" --obj_directory="CAN Devices" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


