sdl_dep = dependency('sdl3', version:'>=3.0.0', required: true)
sdl_image_dep = dependency('sdl3-image', required: true)
nlohmann_json_dep = dependency('nlohmann_json', required: true, fallback: ['nlohmann_json', 'nlohmann_json_dep'])
srcs = ['main.cpp', 'PetEngine.cpp', 'Timer.cpp']
executable('Desktop Pets', srcs, dependencies: [sdl_dep, sdl_image_dep, nlohmann_json_dep], include_directories:inc)
