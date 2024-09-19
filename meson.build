project('zintl', ['c', 'cpp'], default_options : ['cpp_std=c++17'], subproject_dir : 'thirdparty')

glad_proj = subproject('glad')
glad_dep = glad_proj.get_variable('glad_dep')

glreg_proj = subproject('glregistry')
glreg_dep = glreg_proj.get_variable('glreg_dep')

gl_dep = dependency('GL', required : true)

subdir('engine')
subdir('test')
