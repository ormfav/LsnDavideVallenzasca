# Commands needed
* `cd XY`, where XY is the excercises number
* `make XYz` to compile the XYz excercise (a list of 
  compilable excercise can be optained by `ls src/`)
* `make` to compile all exercises for the current folder
* `make clean` remove all object and executable files.
* `bin/XYz.x` to run the selected excercise

* `source lsn` to load some useful aliases for the 
  current shell session. They are:
** `run-XYz` to run the selected excercise
** `mkrun-XYz` to make and run the selected excercise
** `clean-output` to remove all files in `out/`

**note:** these aliases won't work for excercise 10
because there you have to run the executables through `mpiexec`
and select the number of nodes needed for parallel computing

# Folder structure
Each excercise folder is labelled with a two-chars name indicating
the number of the excercise. The structure of the exercise folder is the
following:
```
XX<br>
├── XX.ipynb
├── LSN_Exercises_XX.ipynb
│
├── in
├── out
├── store
├── savestate
│
├── include
├── src
├── obj
├── bin
│
├── lsn
└── makefile
```

- XX.ipynb: jupyter notebook with the results of the excercises
- LSN_Exercises_XX.ipynb: jupyter notebook with the assignements
- in: the folder containing input data (starting values,
  configurations etc..)
- out: a folder with the data collected after the c++ simulation
- store: a folder that collects data we want for sure not to be
  overwritten. Programs can't write in this directory, it's the
  user that manages it
- savestate: a folder with all the information to restart the
  simulation such as random seed or particles positions
- include: folder with the header files for the current excercise
- src: source code folder
- bin: folder containing the executables
- obj: object files folder
- lsn: a file with some useful aliases

In addition to excercises folder you will find a lib folder, containing classes and
functions visible to all the excercises:
```
lib<br>
├── DataBlocking
├── Ga
├── Metropolis
├── Misc
├── Point
└── Random
```
Names are almost self-explanatory. We just whish to point out that:
- Ga stands for "Genetic algorithm"
- Misc are some miscellaneous function we fund ourself using often


