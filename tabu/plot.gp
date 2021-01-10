reset
set terminal png size 800,600
set output "out.png"
set autoscale
comment = "`tail -n 2 out.txt | sed 'N;s/\n/,\ and takes\ /g'`"

set title comment
plot "out.txt" using ($2):($3) with linespoint title ' '
