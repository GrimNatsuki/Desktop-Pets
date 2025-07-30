sdl_dep = dependency('sdl3', version:'>=3.0.0')
sdl_image_dep = dependency('sdl3-image')
srcs = ['main.cpp','Launcher.cpp','PetEngine.cpp','configParser.cpp' ,'RNG.cpp']
executable('Desktop Pets', srcs, dependencies: [sdl_dep, sdl_image_dep], include_directories:inc)

configure_file(
    input:'PetEngine.conf',
    output:'PetEngine.conf',
    copy:true
)