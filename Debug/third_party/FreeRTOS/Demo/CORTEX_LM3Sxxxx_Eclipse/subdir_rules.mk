################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Each subdirectory must supply rules for building sources it contributes
build-1996166351:
	@$(MAKE) --no-print-directory -Onone -f third_party/FreeRTOS/Demo/CORTEX_LM3Sxxxx_Eclipse/subdir_rules.mk build-1996166351-inproc

build-1996166351-inproc: ../third_party/FreeRTOS/Demo/CORTEX_LM3Sxxxx_Eclipse/fury_ft2232.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/Applications/ti/ccs1220/xdctools_3_62_01_16_core/xs" --xdcpath= xdc.tools.configuro -o configPkg -r debug -c "/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-1996166351 ../third_party/FreeRTOS/Demo/CORTEX_LM3Sxxxx_Eclipse/fury_ft2232.cfg
configPkg/compiler.opt: build-1996166351
configPkg/: build-1996166351

build-964758081:
	@$(MAKE) --no-print-directory -Onone -f third_party/FreeRTOS/Demo/CORTEX_LM3Sxxxx_Eclipse/subdir_rules.mk build-964758081-inproc

build-964758081-inproc: ../third_party/FreeRTOS/Demo/CORTEX_LM3Sxxxx_Eclipse/fury_ft2232_flash.cfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: XDCtools'
	"/Applications/ti/ccs1220/xdctools_3_62_01_16_core/xs" --xdcpath= xdc.tools.configuro -o configPkg -r debug -c "/Applications/ti/ccs1220/ccs/tools/compiler/ti-cgt-arm_18.12.8.LTS" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

configPkg/linker.cmd: build-964758081 ../third_party/FreeRTOS/Demo/CORTEX_LM3Sxxxx_Eclipse/fury_ft2232_flash.cfg
configPkg/compiler.opt: build-964758081
configPkg/: build-964758081


