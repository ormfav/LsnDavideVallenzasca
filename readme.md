# TL;DR
Go in the "home" directory. From here you can
* Compile: make <ex>, where <ex> can be: 
    - empty = compile all
    - XY = compile excercise XY
    - XYz = compile assignment z from excercise XY
* Execute: ./lsn XYz (**note:** if you change a .inl input file you
  can compile and execute the desired assignment using ./lsn -c XYz)

* Clean: 

**Note:** An exeption is made for exercise 04: here 041 refers to
the calculation made for the solid phase, 042 for the liquid phase,
043 for the gasseous phase -and not for the actual assignment.

# Folder structure
Each excercise folder is labelled with a two-chars name indicating
the number of the excercise. The structure of the exercise folder is the
following:
```
XX<br>
├── XX.ipynb
├── LSN_Exercises_XX.ipynb
├── bin<br>
│   └── \*.x
├── in<br>
│   └── \*-conf.inl
├── out<br>
│   ├── \*.dat
│   └── \*.csv
├── obj<br>
│   └── \*.o
└── src<br>
    └── \*.cpp
```

- XX.ipynb: jupyter notebook with the results of the excercises
- LSN_Exercises_XX.ipynb: jupyter notebook with the assignements
- bin: folder containing the executables
- in: the folder containing input data
- out: a folder with the data collected after the c++ simulation
- obj: object files folder
- src: source code folder

In addition you will find a lib folder, containing classes and
functions used throughut (almost) all exercises.

# Libraries
## Point
```point<T,N>``` is a template class that aims to rapresent an
N-dimensional point of type T whith its law of motion. 
### Members
- an STL array<T,N> representing the point
- a function ```point<T,N>& -> bool``` that makes the point evolve
  and returns True if the move is accepted (useful when
  implementing the metropolis alghotithm)
- a counter for the total of moves
- a counter for accepted moves
### Methods
- Two contructor: one accepts just the law of motion and place the
  point in the origin, the other alse accepts an array representing
  the starting point
- an operator [] to access the values in the array
_ ```array<T,N>& ToArray()``` that returns a reference to the array
- ```void Move()``` which makes the point evolve with the law of
  motion and updates the counters
- ```void Jump(array<T,N>)``` that lets the object jump directly to
  a particular point in the space. **Note:** this does not count as
  a move
- ```double Lenght()``` and ```double Lenght2()``` returns the norm
  and the norm squared of the point

## DataBlocking
```datablocking<N,M>``` is a class that implements the general
datablocking procedure, evaluating relevant statistical quantities
for M properties based on the values of N evolving parameters.
### Members
- a ```point<double,N>``` representing the set of evolving parameters
- an ```array<double,M>``` representing the properties to evaluate
- an M-dimensional array of functions ```point<double,M> -> double```
  containing the function to evaluate each property from the
  parameters
-
-
- 
### Methods
- Two constructors. They accept:
    - moves per block
    - (optional) an N-dimenional array reresenting the starting
      point. If not provided all parameters are initialized to 0
    - a function ```point<double,N>& -> bool```, the law of
      evolution for parameters 
    - the M-dimensional array of function needed to evaluate the
      properties
- void Measure() makes the point of parameters evolve according to
  the law provided and evaluate, evaluate the properties and
  updates the accumulators for the avereage
- void EvalBlk() 
