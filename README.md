# Wolf 3D

This is a raytracing engine a la Wolfenstein 3D (1992) and similar games.

I made this initially in one week in October 2017 as a project in 42 Silicon Valley. The program uses a map file in the maps folder (currently there is only one) to generate an environment. The environment is then rendered from a first person perspective for the player.

The player can move using the WASD keys, and rotate using the Left and Right arrow keys or the mouse. The player can also hold Left Shift to run. M mutes or unmutes the sound. Tab toggles the minimap Esc exits. (Note: as the allowed libraries and functions used for this project only used keycodes, the key press events are keyboard specific, most likely working only on an Apple 109 key keyboard. This will be fixed shortly.)

This project was initially made with my own libc (in libs/libft), and a gimped version of XLib (libs/MiniLibX; different versions are used ) provided by the school. As I make upgrades to this I will replace the MLX functions with full-fledged XLib functions, and will switch to the standard C library.

To compile the program, download or clone the repository and run `make`. Then run `./wolf3d maps/test.wolf` to test.
