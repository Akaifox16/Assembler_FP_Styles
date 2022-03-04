#!/bin/zsh

cd build

cmake ..

make

./Assembler ../test/assembly ../test/machinecode/machine
./Assembler ../test/factorial ../test/machinecode/factorial_machine
./Assembler ../test/cnr ../test/machinecode/cnr_machine

cd ..

g++ -o old old.cpp

./old ./test/assembly ./test/machinecode/old_machine
./old ./test/factorial ./test/machinecode/old_factorial_machine
./old ./test/cnr ./test/machinecode/old_cnr_machine
