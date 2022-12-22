cmake_minimum_required(VERSION 3.2.0)
project(ZeroPilot C CXX ASM)

set(ELF_NAME ${PROJECT_NAME}.elf)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)


set(ATTITUDE_MANAGER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/AttitudeManager")
set(LAMINAR_OS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS")


add_subdirectory(${LAMINAR_OS_DIR})

get_target_property(LAMINAR_OS_INCLUDE_PATHS LaminarOS LOS_INCLUDES)
message("laminar os includes: ${LAMINAR_OS_INCLUDE_PATHS}")

include_directories(
    ## ZP Software Includes
    ${CMAKE_CURRENT_SOURCE_DIR}/AttitudeManager/Inc
    ${CMAKE_CURRENT_SOURCE_DIR}/AttitudeManager/ControlAlgorithms/Inc

    ${CMAKE_CURRENT_SOURCE_DIR}/SystemManager/Inc

    ${CMAKE_CURRENT_SOURCE_DIR}/Common/Inc

    ${LAMINAR_OS_INCLUDE_PATHS}
)

set(ZP_SM ${CMAKE_CURRENT_SOURCE_DIR}/SystemManager)
set(ZP_SM_C_SOURCES "${ZP_SM}/Src/*.c")
set(ZP_SM_CXX_SOURCES "${ZP_SM}/Src/*.cpp")

set(ZP_AM ${CMAKE_CURRENT_SOURCE_DIR}/AttitudeManager)
set(ZP_AM_C_SOURCES "${ZP_AM}/Src/*.c"
                    "${ZP_AM}/ControlAlgorithms/Src/*.c")
set(ZP_AM_CXX_SOURCES "${ZP_AM}/Src/*.cpp"
                      "${ZP_AM}/ControlAlgorithms/Src/*.cpp")

file(GLOB_RECURSE C_SOURCES ${ZP_SM_C_SOURCES}
                            ${ZP_AM_C_SOURCES})
message("MESSAGE: ${C_SOURCES}")
file(GLOB_RECURSE CXX_SOURCES ${ZP_SM_CXX_SOURCES}
                              ${ZP_AM_CXX_SOURCES})

set(STARTUP_ASM_FILE ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/${STARTUP_ASM})
set(LINKER_SCRIPT_FILE ${CMAKE_CURRENT_SOURCE_DIR}/LaminarOS/${LINKER_SCRIPT})

add_executable(${ELF_NAME} main.cpp ${C_SOURCES} ${CXX_SOURCES} ${STARTUP_ASM_FILE})

target_link_libraries(${ELF_NAME} LaminarOS)

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
