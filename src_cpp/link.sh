#!/bin/bash
g++ -Iinclude ida_star.o eph.o cph.o eoh.o lehmer_code.o cube.o main.o -o main
g++ -Iinclude lehmer_code.o cube.o eph_gen.o -o eph_gen
g++ -Iinclude lehmer_code.o cube.o cph_gen.o -o cph_gen
g++ -Iinclude lehmer_code.o cube.o eoh_gen.o -o eoh_gen
