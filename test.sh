#!/bin/zsh

cd build

cmake ..

make

./Assembler ../test/assembly ../test/machinecode/machine
./Assembler ../test/factorial ../test/machinecode/factorial_machine
./Assembler ../test/cnr ../test/machinecode/cnr_machine