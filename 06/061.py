import sys
import numpy as np
import subprocess

Tmin=0.5
Tmax=2
npoints=50


if len(sys.argv) != 3:
    raise ValueError(
'''Specify sampling alghorythm: 
0 - Gibbs
1 - Metropolis 
Specify external field value'''
    )
if sys.argv[1]=='0':
    alg="gibbs"
elif sys.argv[1]=='1':
    alg="mrt2"
else: 
    raise ValueError(
'''Valid args: 
1 - Metropolis 
2 - Gibbs'''
    )

suffix=["ene","heat","mag","chi"]
files = [open("store/061-"+alg+"_"+x+"-h="+sys.argv[2],'w+') for x in suffix]
    


for T in np.linspace(Tmin,Tmax,npoints):
    print("Running simulation with: T =",T,", h =",sys.argv[2])
    with open('in/input.dat', 'r') as f:
        sed=f.readlines()
    sed[0]=str(sys.argv[1])+"\n"
    sed[2]=str(T)+"\n"
    sed[5]=str(sys.argv[2])+"\n"
    
    with open('in/input.dat', 'w') as f:
        f.writelines(sed)
    
    subprocess.run("bin/061.x",stdout=subprocess.DEVNULL)

    for i in range(4):
        with open("out/output."+suffix[i]+".0") as f:
            last_line = f.readlines()[-1]
        files[i].write(last_line)
    
    print("--------")


