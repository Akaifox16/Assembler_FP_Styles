#!/bin/zsh

cd build

cmake ..

make

./Assembler ../test/cnr ../test/machinecode/cnr_machine