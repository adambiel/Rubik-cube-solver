#!/bin/bash

echo Starting to generate eph data:
./eph_gen >../data/eph.txt

echo Starting to generate eoh data:
./eoh_gen >../data/eoh.txt

echo Starting to generate cph data:
./cph_gen >../data/cph.txt

#rm *.o
rm cph_gen
rm eph_gen
rm eoh_gen

