set title 'Benchmark of pthread_alloc/pthread_join'
set xlabel 'number of threads sleeping'
set ylabel 'time [us]'
set key right bottom
plot 'create.csv' using 1:2 title 'pthread_create',\
  'join.csv' using 1:2 title 'pthread_join'
