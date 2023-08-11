set(_core_sources .)
set(_core_linker_script ${BSP_ROOT}/STM32F103C6TX_FLASH.ld)

set(_core_includes .)

set(_core_defines USE_HAL_DRIVER STM32F103x6)

set(_core_commonflags -mcpu=cortex-m3)

set(_core_ID STM32F103C6Tx)

