engine_sources = []

subdir('framework')
subdir('platform')

dwm_dep = declare_dependency(link_args : ['dwmapi.lib'])

engine_inc = include_directories('include')
engine_lib = static_library('zintl', engine_sources, include_directories : engine_inc, dependencies : [gl_dep, glad_dep, glreg_dep, dwm_dep])
engine_dep = declare_dependency(link_with : engine_lib, include_directories : engine_inc)
