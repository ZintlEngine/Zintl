current_sources = [
    'wgl/context.cpp',
    'win32/application.cpp',
    'win32/window.cpp',
]
current_dir = meson.current_source_dir()

foreach file : current_sources
    path = join_paths(current_dir, file)
    engine_sources += files(path)
endforeach
