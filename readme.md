# TL;DR
Go in the "home" directory. From here you can
* Compile: make <ex>, where <ex> can be: 
    - empty = compile all
    - XY = compile excercise XY
    - XYz = compile assignment z from excercise XY
* Execute: ./lsn XYz (**note:** if you change a .inl input file you
  can compile and execute the desired assignment using ./lsn -c XYz)
* View results:

* Clean: 
**Note:** An exeption is made for exercise 04: here 041 refers to
the calculation made for the solid phase, 042 for the liquid phase,
043 for the gasseous phase -and not for the actual assignment.

# Folder structure
Each excercise folder is labelled with a two-chars name indicating
the number of the excercise. The structure of the folder is the
following:

XX<br>
├── XX.ipynb<br>
├── LSN_Exercises_XX.ipynb<br>
├── bin<br>
│   └── \*.x<br>
├── config<br>
│   └── \*-conf.inl<br>
├── Data<br>
│   ├── \*.dat<br>
│   └── \*.csv<br>
├── obj<br>
│   └── \*.o<br>
└── src<br>
    └── \*.cpp<br>

- XX.ipynb: jupyter notebook with the results of the excercise
- LSN_Exercises_XX.ipynb: jupyter notebook with the assignements of
  the excercise
- bin: the folder containing the executables
- config: the folder containing the 
- Data: a folder with the data collected after the c++ simulation
- obj: folder with the object files
- src: folder with the source code

