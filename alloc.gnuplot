set title 'Benchmark of free/malloc'
set xlabel 'size [B]'
set ylabel 'time [us]'
set key right bottom
set logscale x
plot 'free.csv' using 1:2 title 'free',\
  'malloc.csv' using 1:2 title 'malloc',\
  'calloc.csv' using 1:2 title 'calloc',\
  'sbrk.csv' using 1:2 title 'sbrk',\
  'brk.csv' using 1:2 title 'brk'
