project('glad', 'c')

glad_inc = include_directories('include')
glad_lib = library('glad', sources : ['src/gl.c'], include_directories : glad_inc)
glad_dep = declare_dependency(link_with : glad_lib, include_directories : glad_inc)
