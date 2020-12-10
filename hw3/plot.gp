reset
set terminal png size 800,600
set output "output.png"
set autoscale
comment = "`tail -n 2 output.txt | sed 'N;s/\n/,\ and takes\ /g'`"

set title comment
plot "output.txt" using ($2):($3) with linespoint title ' '
