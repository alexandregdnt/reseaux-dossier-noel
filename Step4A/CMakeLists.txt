cmake_minimum_required(VERSION 3.20)
project(Step4A C)

set(CMAKE_C_STANDARD 99)

include_directories(
        Libraries
        Libraries/udplib
)

add_executable(Step4A
        Libraries/data.h
        Libraries/LibSerAG.h
        Libraries/requeteAG.h
        Libraries/LibSerAG.c
        Libraries/udplib/udplib.h
        Libraries/udplib/udplib.c
        VehiculesAG
        FacturesAG
        admin.c)
