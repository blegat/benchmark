#!/bin/bash

i=0
a=0

while ((i <= $1))
do
expr ((a=a+1))
((i += 1))
done 
