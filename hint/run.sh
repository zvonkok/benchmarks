#!/bin/sh
# Made by Rajat Todi 
#
# Assumption is that you have already DOUBLE and INT in the
# directory and also a directory called 'data' inside.
#
#
date
#
#
echo "Running for serial processor on different datatypes..."
#
#
mkdir data$1 || exit 1
echo "double"
./DOUBLE $1 > ./data$1/double.out
echo "int"
./INT $1 > ./data$1/int.out
#./SHORT > ./data/s1_out
echo "float"
./FLOAT $1 > ./data$1/float.out
#./LONGLONG > ./data/l1_out
#
#
echo "FINISH FOR SERIAL"
