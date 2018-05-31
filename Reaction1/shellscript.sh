#!/bin/bash



rm movie.xyz
rm Energyprofile.png
module load cp2k/5.0
cp2k.popt -i neb2.inp -o neb2.out

### Define the number the replicas in the following for loops (both loops). This script is prepared for 50 replica simulation. Modify 50 according to your given conditions.
for a in {01..50} ; do tail -n 9 neb2-pos-Replica_nr_${a}-1.xyz >> movie.xyz ; done 
for a in {01..50} ; do grep ENERGY  neb2-BAND${a}.out | tail -n 1 | awk '{print '${a}',$9}' ; done > neb2_profile

gnuplot Plot.gp
rm neb2-*
rm neb2.out
rm neb2_profile

