# User Manual for Jetpack Runner

## What is Jetpack Runner?
Jetpack Runner is an Endless runner game inspired by the very well known game from Halfbrick Studios called Jetpack Joyride. The goal of the game is to travel the
longest distance possible while picking up coins and avoiding electric zappers. The character of the game has an air jetpack which allows him to fly. The game starts slow and easy but it gets progressively harder as the speed of the character increases.. If a player hits a zapper, they have no choice but to start again from the beginning.

## How to play
If you want your character to go up and fly, just hold down **space**. If you want the character to run on the ground or just to adjust the characters altitude,
don't press anything and he will fall and after landing he will run on his own. It's that simple! If you keep pressing the spacebar the character will fly as high as he
can and after that he will be dragging his head on the ceiling. Not very pleasant, but sometimes necessary to avoid the zappers.
If you want your character to suffer a very horrible death by electrocuting him, just run or fly into a zapper. The game will stop and you just need to press **R** to revive him and try again. If you already played enough and wish to close the game, just close the window. There are also gravity-reversing candles located throughout the laboratory. Once you pick one up, the gravity reverses.

## Platforms

### For windows
Easiest way to compile the game is by using Visual studio. It should be a matter of opening the .sln file, and compiling the game. The necessary sfml files should download automatically because the game uses NUGET to deal with the downloading and linking of the libraries. If the NUGET packages are not downloaded automatically, open the NUGET menu inside the Visual studio, find [this package](https://www.nuget.org/packages/SFML-cpp/2.5.1?_src=template) and install it. Just make sure that the `Resources` folder is present next to the .exe file. 

### For other OSs
The game can also be compiled using CMake. The required cmake file is already present in the repository. It might be necessary to download sfml-dev package and other necessary packages if you don't have it already installed. You should also have make installed on your system. 

- Type "cmake CMakeLists.txt" inside the folder where the CMakeList.txt is located
- Type "make"
All necessary files should be compiled and an executable should be created. To run the game, type "./JetpackRunner".

Graphical assets provided by: [mcguy](https://www.mcguy.org/graphics)

## Programming guide
You can read the programming documentation [here](/Programmer-documentation.md)