# SIMULATED ANNEALING TSP DEMO

This code shows how the simulated annealing optimization algorithm can be 
applied to the traveling salesman problem.

<img src="http://geekstack.net/tsp.gif" width="550" />

## How to compile the project?

First, create a build directory
```
$ mkdir build
$ cd build
```

Run cmake
```
$ cmake -DCMAKE_BUILD_TYPE=Release ..
```

Run make
```
$ make
```

The executable is located in the bin/ subdirectory and is named "sa". Run the 
program as follows
```
$ ./sa
```

## How do I change the parameters?

All important parameters are defined in the main.cpp file. There you can adjust
the number of iterations, the cooling schedule, and the screen update cycle. 
Remember to recompile the project once you are finished updating the parameters. 

## What problem do we solve?

- new-york street map, this is for benchmark purposes because we know the exact solution here
- generate a random map
- read in a tsp-file (discussed below)

If you run the program without any parameters, then a new-york street map is generated.

**Call the program from console with parameter `--help` to get more information.**

-----
You can test different datasets from the LIBTSP repository [1]. 
Simply provide the filename of the .tsp file as the first argument. You 
almost surely have to adjust the parameters in order to get a good approximation.

You can use [berlin52.tsp](http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp/berlin52.tsp.gz) 
for a quick test. The example should run well without needing to adjust the parameters.

You can compare your results (using your parameters settings) to the optimal result [2]. 

The program only works with instances of type TSP and edge weight type EUC_2D. 

##Implemented cooling schedules

- Geometric schedule
- logarithmic schedule [8]
- constant thermodynamic speed in order 1 [6-10]

## What do the lines represent?

The yellow line shows the shortest cycle that has been found so far. The purple
line shows the current state. 

## Resources

[1]: TSPLIB95, University of Heidelberg. http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/tsp/

[2]: TSPLIB95 best solutions found, University of Heidelberg. http://comopt.ifi.uni-heidelberg.de/software/TSPLIB95/STSP.html

[3]: D. Bertsimas and J. Tsitsiklis. Simulated Annealing. Statistical Science Vol. 8 No. 1, pages 10-15, 1993

[4]: D. Stroock. An Introduction to Markov Processes. Graduate texts in mathematics,  Springer, 2005. 

[5]: C. Geyer. Markov Chain Monte Carlo Lecture Notes. http://www.stat.umn.edu/geyer/f05/8931/n1995.pdf

[6]: Yaghout Nourani and Bjarne Andresen 1998 J. Phys. A: Math. Gen. 31 8373. https://iopscience.iop.org/article/10.1088/0305-4470/31/41/011

[7]: Andresen B., Gordon J.M. (1996) Constant thermodynamic speed simulated annealing. In: Jacobsen B.H., Mosegaard K., Sibani P. (eds) Inverse Methods. Lecture Notes in Earth Sciences, vol 63. Springer, Berlin, Heidelberg. https://doi.org/10.1007/BFb0011789

[8]: Performance Analysis of Simulated Annealing Cooling Schedules in the Context of Dense Image Matching https://www.doi.org/10.13053/CyS-21-3-2553

[9]: Salamon et al., Simulated annealing with constant thermodynamic speed. https://doi.org/10.1016/0010-4655(88)90003-3

[10]: Abdellatif El Afia et al., A Self Controlled Simulated Annealing Algorithm using Hidden Markov Model State Classification. https://doi.org/10.1016/j.procs.2019.01.024




#License
This is a fork of https://github.com/TobyPDE/simulated-annealing-tsp

**Thanks a lot!**

The MIT License (MIT) Copyright (c) 2021 Babalion
## Original License

The MIT License (MIT) Copyright (c) 2016 Tobias Pohlen

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
