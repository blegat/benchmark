set title 'Benchmark of open, write, read, close'
set xlabel 'size of the file at opening or length of the string to read/write'
set ylabel 'time [us]'
set key right bottom
plot 'open.csv' using 1:2 title 'open',\
  'write.csv' using 1:2 title 'write',\
  'read.csv' using 1:2 title 'read',\
  'close.csv' using 1:2 title 'close'
