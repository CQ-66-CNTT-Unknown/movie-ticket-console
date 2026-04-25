CMAKE_MINIMUM_REQUIRED(VERSION 4.0)

project(movie_ticket_console VERSION 1.0.0 LANGUAGES C)

if(NOT CMAKE_C_COMPILER_ID STREQUAL "GNU")
    message(WARNING "This project is configured for GCC. Current compiler: ${CMAKE_C_COMPILER_ID}")
endif()

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

set(PROJECT_SOURCES
    src/main.c
    src/services/auth.c
    src/services/customer_services.c
    src/services/manager_services.c
    src/services/movie_services.c
    src/services/screening_services.c
    src/utils/show_actions_menu.c
)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

target_include_directories(${PROJECT_NAME}
    PRIVATE
        ${PROJECT_SOURCE_DIR}/include
)

if(CMAKE_C_COMPILER_ID STREQUAL "GNU")
    target_compile_options(${PROJECT_NAME}
        PRIVATE
            -Wall
            -Wextra
            -Wpedantic
    )
endif()
