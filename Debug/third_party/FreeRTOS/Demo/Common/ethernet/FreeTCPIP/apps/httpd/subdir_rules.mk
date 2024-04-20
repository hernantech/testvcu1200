################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
third_party/FreeRTOS/Demo/Common/ethernet/FreeTCPIP/apps/httpd/%.obj: ../third_party/FreeRTOS/Demo/Common/ethernet/FreeTCPIP/apps/httpd/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: Arm Compiler'
	"/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=FPv4SPD16 -me -Ooff --opt_for_speed=5 --fp_mode=strict --include_path="/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS/include" --include_path="/Users/alexhernandez/workspace_june_23/VCU1200_Testbench" --include_path="C:/TI/TivaWare_C_Series-2.1.4.178" --define=ccs="ccs" --define=PART_TM4C123BH6PGE -g --gcc --diag_warning=225 --diag_wrap=off --display_error_number --abi=eabi --preproc_with_compile --preproc_dependency="third_party/FreeRTOS/Demo/Common/ethernet/FreeTCPIP/apps/httpd/$(basename $(<F)).d_raw" --obj_directory="third_party/FreeRTOS/Demo/Common/ethernet/FreeTCPIP/apps/httpd" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


