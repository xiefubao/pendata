project(main)
cmake_minimum_required(VERSION 2.6)
aux_source_directory(. dir_srcs)
add_executable(main ${dir_srcs})
