#!/bin/bash

time g++ -o Verwirrrer main.cpp -O3 -march=native
cp -v Verwirrrer testumgebung/
