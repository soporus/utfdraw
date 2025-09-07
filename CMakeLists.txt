cmake_minimum_required(VERSION 3.31.6)
project(utfdraw)

add_executable(utfdraw
  main.c
  term.c
  term.h
  termbox2/termbox2.h
)
add_compile_options($(PROJECT_NAME)
  -std=gnu23
  -O2 
  -s 
  -Wconversion 
  -Wno-sign-conversion 
  -Wdouble-promotion 
  -flto=full 
  -march=native 
  -Wpedantic 
  -DTB_LIB_OPTS=1 
  -DTB_OPT_ATTR_W=32 
  -DTB_OPT_PRINTF_BUF=512
)
add_link_options($(PROJECT_NAME)
  -std=gnu23
  -O2 
  -s 
  -Wconversion 
  -Wno-sign-conversion 
  -Wdouble-promotion 
  -flto=full 
  -march=native 
  -Wpedantic 
  -DTB_LIB_OPTS=1 
  -DTB_OPT_ATTR_W=32 
  -DTB_OPT_PRINTF_BUF=512
)

