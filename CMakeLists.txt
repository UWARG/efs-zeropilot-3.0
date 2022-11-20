cmake_minimum_required(VERSION 3.2.0)
project(ZeroPilot C CXX ASM)

set(ELF_NAME ${PROJECT_NAME}.elf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    ## ZP Software Includes
    ${CMAKE_CURRENT_SOURCE_DIR}/AttitudeManager/Inc

    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Interface/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Core/Inc

    ## LOS Driver Includes ##
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/Common/${FOLDER_NAME}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/RC_Receiver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/Airspeed/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/Altimeter/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/Gps/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/IMU/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/SensorFusion/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver/Motor_Channel/Inc

    ## LOS Boardfiles ##
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Drivers/${FAMILY_NAME}_HAL_Driver/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Core/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Drivers/CMSIS/Device/ST/${FAMILY_NAME}/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Drivers/CMSIS/Include
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_${PORTABLE_NAME}
    ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source/include
)

set(HAL_DRIVERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Drivers)
set(HAL_DRIVERS_C_SOURCES "${HAL_DRIVERS_DIR}/${FAMILY_NAME}_HAL_Driver/Src/*.c")

set(FREE_RTOS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Middlewares/Third_Party/FreeRTOS/Source)
set(FREE_RTOS_C_SOURCES "${FREE_RTOS_DIR}/*.c"
                        "${FREE_RTOS_DIR}/CMSIS_RTOS/*.c"
                        "${FREE_RTOS_DIR}/portable/GCC/ARM_${PORTABLE_NAME}/*.c"
                        "${FREE_RTOS_DIR}/portable/MemMang/*.c")

set(HAL_CORE ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/boardfiles/${FOLDER_NAME}/Core)
set(HAL_CORE_C_SOURCES "${HAL_CORE}/Src/*.c")
set(HAL_CORE_CXX_SOURCES "${HAL_CORE}/Src/*.cpp")

set(INTERFACE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Interface)
set(INTERFACE_C_SOURCES "${INTERFACE_DIR}/Src/*.c")
set(INTERFACE_CXX_SOURCES "${INTERFACE_DIR}/Src/*.cpp")

set(DRIVERS_DIR ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Driver)
set(DRIVERS_C_SOURCES "${DRIVERS_DIR}/*/Src/*.c")
set(DRIVERS_CXX_SOURCES "${DRIVERS_DIR}/*/Src/*.cpp")

set(CORE_Dir ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/LOS_Core)
set(CORE_C_SOURCES "${CORE_Dir}/Src/*.c")
set(CORE_CXX_SOURCES "${CORE_Dir}/Src/*.cpp")

file(GLOB_RECURSE C_SOURCES ${HAL_DRIVERS_C_SOURCES}
                            ${HAL_CORE_C_SOURCES}
                            ${INTERFACE_CXX_SOURCES}
                            ${DRIVERS_CXX_SOURCES}
                            ${CORE_C_SOURCES}
                            ${FREE_RTOS_C_SOURCES})                            
message("MESSAGE: ${C_SOURCES}")                         
file(GLOB_RECURSE CXX_SOURCES ${HAL_CORE_CXX_SOURCES} 
                              ${INTERFACE_CXX_SOURCES}
                              ${DRIVERS_CXX_SOURCES}
                              ${CORE_CXX_SOURCES})

set(STARTUP_ASM_FILE ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/${STARTUP_ASM})
set(LINKER_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/${LINKER_SCRIPT})

add_executable(${ELF_NAME} main.cpp ${C_SOURCES} ${CXX_SOURCES} ${STARTUP_ASM_FILE})

# Add project-specific linker flags (.ld script, .map file)
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -T${LINKER_SCRIPT_FILE} -Wl,-Map=${PROJECT_BINARY_DIR}/${PROJECT_NAME}.map,--cref")

set(BIN_FILE ${PROJECT_BINARY_DIR}/${PROJECT_NAME}.bin)

# objcopy the .elf file to .bin file for programming
add_custom_target("${PROJECT_NAME}.bin" ALL
  COMMAND ${CMAKE_OBJCOPY} -Obinary $<TARGET_FILE:${ELF_NAME}> ${BIN_FILE}
  DEPENDS ${ELF_NAME}
)

# Print size information after compiling
add_custom_command(TARGET ${ELF_NAME}
  POST_BUILD
  COMMENT "Binary size information:"
  COMMAND ${CMAKE_SIZE} $<TARGET_FILE:${ELF_NAME}>
)

# the following is windows only
set(INSTALL_CMD "ST-LINK_CLI")
set(INSTALL_OPTS "-c SWD -P \"${BIN_FILE}\" 0x08000000 -NoPrompt -Rst -Run")

install(CODE
  "execute_process(
    COMMAND ${INSTALL_CMD} ${INSTALL_OPTS}
  )"
)

message("CMAKE_BUILD_TYPE = ${CMAKE_BUILD_TYPE}")

IF(CMAKE_BUILD_TYPE MATCHES Debug)
    message("Debug build.")
ELSEIF(CMAKE_BUILD_TYPE MATCHES Release)
    message("Release build.")
ELSE()
    message("Some other build type.")
ENDIF()

add_custom_target(UPLOAD
        arm-none-eabi-gdb -iex "target remote tcp:127.0.0.1:3333"
        -iex "monitor program $<TARGET_FILE:${PROJECT_NAME}.elf>"
        -iex "monitor reset init"
        -iex "disconnect" -iex "quit")
