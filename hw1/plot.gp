reset
set terminal png size 800,600
set output "output.png"
set autoscale

plot "output.txt" using ($2):($3) with linespoint title ' '