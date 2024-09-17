project('zintl', 'cpp', default_options : ['cpp_std=c++17'], subproject_dir : 'thirdparty')

glheaders_proj = subproject('openglheaders')
glheaders_dep = glheaders_proj.get_variable('gl_dep')

subdir('engine')
subdir('test')
