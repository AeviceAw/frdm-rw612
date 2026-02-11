################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/mcuxCsslParamIntegrity32.c 

C_DEPS += \
./component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/mcuxCsslParamIntegrity32.d 

OBJS += \
./component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/mcuxCsslParamIntegrity32.o 


# Each subdirectory must supply rules for building sources it contributes
component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/%.o: ../component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/%.c component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -std=gnu99 -D__REDLIB__ -DCPU_RW612ETA2I -DCPU_RW612ETA2I_cm33_nodsp -DMCUXPRESSO_SDK -DFSL_SDK_DRIVER_QUICK_ACCESS_ENABLE=1 -DSDK_DEBUGCONSOLE=1 -DMCUX_META_BUILD -DRW612_SERIES -DBOOT_HEADER_ENABLE=1 -DCR_INTEGER_PRINTF -DPRINTF_FLOAT_ENABLE=0 -D__MCUXPRESSO -D__USE_CMSIS -DDEBUG -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\source" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\flash_config" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\drivers" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\CMSIS" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\CMSIS\m-profile" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\device" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\device\periph" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\utilities" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\utilities\str" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\utilities\debug_console_lite" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\uart" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClBuffer\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClBuffer\inc\internal" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClCore\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClEls\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClEls\inc\internal" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClMemory\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxClMemory\inc\internal" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslMemory\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslMemory\inc\internal" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslCPreProcessor\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslDataIntegrity\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslFlowProtection\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslParamIntegrity\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\comps\mcuxCsslSecureCounter\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\compiler" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\platforms\rw61x" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\src\platforms\rw61x\inc" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\component\els_pkc\includes\platform\rw61x" -I"C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\board" -O0 -fno-common -g3 -gdwarf-4 -c -ffunction-sections -fdata-sections -fno-builtin -mcpu=cortex-m33+nodsp -imacros "C:\MCUexpresso\aeviceMD12\rdrw612bga_flexcomm_spi_polling_b2b_transfer_master\source\mcux_config.h" -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m33+nodsp -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -D__REDLIB__ -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-component-2f-els_pkc-2f-src-2f-comps-2f-mcuxCsslParamIntegrity-2f-src

clean-component-2f-els_pkc-2f-src-2f-comps-2f-mcuxCsslParamIntegrity-2f-src:
	-$(RM) ./component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/mcuxCsslParamIntegrity32.d ./component/els_pkc/src/comps/mcuxCsslParamIntegrity/src/mcuxCsslParamIntegrity32.o

.PHONY: clean-component-2f-els_pkc-2f-src-2f-comps-2f-mcuxCsslParamIntegrity-2f-src

