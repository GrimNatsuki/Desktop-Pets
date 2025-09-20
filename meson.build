project('Desktop Pets', 'cpp')
inc = include_directories('include')

sdl_dep = dependency('sdl3', version:'>=3.0.0', required: true)
sdl_image_dep = dependency('sdl3-image', required: true)
nlohmann_json_dep = dependency('nlohmann_json', required: true, fallback: ['nlohmann_json', 'nlohmann_json_dep'])
srcs = ['src/main.cpp', 'src/PetEngine.cpp', 'src/Timer.cpp']
executable('desktop-pets', srcs, dependencies: [sdl_dep, sdl_image_dep, nlohmann_json_dep], include_directories:inc, link_args: ['-static-libgcc', '-static-libstdc++'])

subdir('tex')
subdir('conf')

