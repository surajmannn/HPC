The deqn C++ mini program models heat transfer through a plane using a partial differential equation. 
The partial differential equation is realized as the program discretizes both time and space by dividing the special domain into a grid of cells from which the program diffuses the temperature from each cell to its neighbouring cell based on the heat equation. 
This diffusion process simulates how heat is spread from hotter regions to cooler regions (cells) before reaching an equilibrium.
The goal of project was to explore and implement how the use of parallelization techniques can potentially improve the efficiency and as such, 
the runtime of the mini program through optimization using the OpenMP API. 
