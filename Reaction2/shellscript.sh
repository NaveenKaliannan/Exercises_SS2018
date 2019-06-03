#!/bin/bash

rm movie.xyz
rm Energyprofile.png

module load cp2k/5.0
cp2k.popt -i neb.inp -o neb.out

### Define the number the replicas in the following for loops (both loops). This script is prepared for 14 replica simulation. Modify 14 according to your given conditions.
for a in {01..28} ; do tail -n 2938 NEB-pos-Replica_nr_${a}-1.xyz >> movie.xyz ; done 
for a in {01..28} ; do grep ENERGY  NEB-BAND${a}.out | tail -n 1 | awk '{print '${a}',$9}' ; done > neb_profile 

rm -rf  NEB-BAND*
gnuplot Plot.gp
