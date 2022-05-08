cmake_minimum_required(VERSION 3.15)
project(TextQuestInter)

link_directories("C:/MinGW/lib/")

set(CMAKE_CXX_STANDARD 17)

add_executable(TextQuestInter
        main.cpp TextPrinter.cpp TextPrinter.hpp json.hpp #Block.cpp Block.hpp
        InputField.cpp InputField.hpp)

if(CMAKE_BUILD_TYPE MATCHES Debug)
    target_link_libraries(TextQuestInter sfml-graphics-d)
    target_link_libraries(TextQuestInter sfml-window-d)
    target_link_libraries(TextQuestInter sfml-system-d)
    target_link_libraries(TextQuestInter sfml-audio-d)
    set(CMAKE_CXX_FLAGS "-Wall -Wextra")
elseif(CMAKE_BUILD_TYPE MATCHES Release)
    target_link_libraries(TextQuestInter sfml-graphics)
    target_link_libraries(TextQuestInter sfml-window)
    target_link_libraries(TextQuestInter sfml-system)
    target_link_libraries(TextQuestInter sfml-audio)
    set(CMAKE_CXX_FLAGS "-mwindows -O3 my.res")
endif()