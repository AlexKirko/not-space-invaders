# Not-quite Space Invaders

This project is a Space Invaders-like game coded in OpenGL and aiming to be cross-platform.

## My God, why this again?

There are good reasons to code a simple game while learning a new programming language. For C++ specifically, it requires abstracting barebone APIs (OpenGL), polymorphism, resource management, and a huge pile of other skills that I would have forgotten without deciding on this project.

By far, the most interesting part of this was writing my own little sprite-drawing graphical engine. I intend to unbundle it from the project into a separate repo, because the OpenGL sprite engines I'd been able to Google either suffer from serious overengineering and feature creep or don't do enough.

I'm a fan of project-based learning and getting your work to a state where you aren't ashamed of showing it, so here it is.

## Installation instructions

This is a Viasual Studio project, and the required external libraries are included. The solution is inside the `SpaceInvadersCPP` folder It should run on Windows NVIDIA after cloning. If not in Windows or running another OS, comment out `#define NVIDIA_GPU` in the main file, and you should be fine. This code forces the program to run on a non-integrated NVIDIA GPU on Windows instead of the pathetic integrated one. You might need something similar on a different system, if the program ends up writing on the cardboard box that is the integrated graphics adapter.

More detailed instructions will follow once I'm done playing with it.