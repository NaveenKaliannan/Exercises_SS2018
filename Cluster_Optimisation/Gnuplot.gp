
# Setting the terminal
set encoding utf8 
set terminal postscript enhanced 
set terminal postscript eps size 3.5,3 enhanced color \
    font 'Arial,14'  linewidth 1.5
#set key box lw 1
#set key width 0.5 height 1
#set key font 'Arial,15'
#set key spacing 2


#plotting
plot "Energy.dat" using 1:2 title "" with points pt 7 ps 0.6 lc 1

# labels and axis
set title "Energy profile as a function of MC-steps"
#set xrange [0:10000000]
set yrange [-1800:-1000]
set xlabel "MC-steps" 
set ylabel "Potential energy (eV)"
set xtics font ",10"
set ytics font ",10"

# Output files
#set terminal postscript eps enhanced size 5.5in,2.2in
#set output 'Energy.eps'
set terminal png font arial 14 size 640,480
set output 'Energy.png'
replot

