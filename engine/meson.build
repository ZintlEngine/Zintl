engine_sources = []

subdir('framework')
subdir('platform')

engine_inc = include_directories('include')
engine_lib = library('zintl', engine_sources, include_directories : engine_inc, dependencies : [glheaders_dep])
engine_dep = declare_dependency(link_with : engine_lib, include_directories : engine_inc)
