#!/bin/bash

types=('short' 'int' 'long int' 'long long int' 'float' 'double' 'long double')
format=('"%hd "' '"%d "' '"%ld "' '"%lld "' '"%f "' '"%lf "'  '"%Lf "')

filec=textbinutil.c
fileh=textbinutil.h

cp textbintemplate.c textbin.c
cp start.c $filec
rm $fileh

for i in ${!types[*]}; do
  type_underscored=$(echo ${types[$i]} | sed 's/ /_/g')
  sed "s/TYPE/${types[$i]}/g" < template.c | sed "s/FORMAT/${format[$i]}/g" | sed "s/POST/${type_underscored}/g" >> $filec
  sed "s/TYPE/${types[$i]}/g" < template.h | sed "s/FORMAT/${format[$i]}/g" | sed "s/POST/${type_underscored}/g" >> $fileh
done
