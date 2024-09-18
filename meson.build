project('zintl', ['c', 'cpp'], default_options : ['cpp_std=c++17'], subproject_dir : 'thirdparty')

glheaders_proj = subproject('openglheaders')
glheaders_dep = glheaders_proj.get_variable('gl_dep')
glad_proj = subproject('glad')
glad_dep = glad_proj.get_variable('glad_dep')

gl_dep = dependency('GL', required : true)

subdir('engine')
subdir('test')
