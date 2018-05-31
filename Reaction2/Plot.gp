
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
plot "neb_profile" using 1:2 notitle with line ls 1 lw 2 lc 1

# labels and axis
set title "Energy profile as a function of replica"
set xlabel "Replica number"
set ylabel "Energy (arb. unit.)"

# Output files
set terminal png font arial 14 size 640,480
set output 'Energyprofile.png'
replot

