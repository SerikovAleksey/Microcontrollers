cmake_minimum_required(VERSION 3.15)

project(led-stairs LANGUAGES C CXX ASM)


#find_bsp(C_STANDARD 99 DISABLE_GNU_EXTENSIONS SOURCE bsp)
find_bsp(ID com.sysprogs.project_importers.stm32.cubemx SOURCE_PROJECT ./../common/led-stairs.ioc)

#
#file(GLOB HAL_SOURCES  "${PROJECT_SOURCE_DIR}/../common/Drivers/STM32F1xx_HAL_Driver/Src/*.c")
#file(GLOB HAL_HEADERS  "${PROJECT_SOURCE_DIR}/../common/Drivers/STM32F1xx_HAL_Driver/Inc/*.h")
#file(GLOB CONF_SOURCES "${PROJECT_SOURCE_DIR}/../common/Core/Src/*.c")
#file(GLOB CONF_HEADERS "${PROJECT_SOURCE_DIR}/../common/Core/Inc/*.h")

list(APPEND PROJECT_SOURCES

        "${PROJECT_SOURCE_DIR}/src/main.c"
        

        #"${HAL_HEADERS}"
        #"${HAL_SOURCES}"
        #"${CONF_HEADERS}"
        #"${CONF_SOURCES}"
        )

list(APPEND INCLUDE_DIRS

    "${PROJECT_SOURCE_DIR}/include"

    "${PROJECT_SOURCE_DIR}/../common/Core/Inc"

    "${PROJECT_SOURCE_DIR}/../common/Drivers/STM32F1xx_HAL_Driver/Inc"
    "${PROJECT_SOURCE_DIR}/../common/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy"

    "${PROJECT_SOURCE_DIR}/../common/Drivers/CMSIS/Device/ST/STM32F1xx/Include"
    "${PROJECT_SOURCE_DIR}/../common/Drivers/CMSIS/Include"
    
  )

    set(NAME_FW "led-stairs")
add_bsp_based_executable(NAME led-stairs
SOURCES ${PROJECT_SOURCES}
	GENERATE_BIN
	GENERATE_MAP)


target_include_directories(${NAME_FW} PUBLIC ${INCLUDE_DIRS})