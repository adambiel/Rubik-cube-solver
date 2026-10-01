#!/bin/bash
for file in *.cpp; 
do
	echo compiling $file
	g++ -Iinclude -O3 $file -c
done
