#!/bin/zsh

cd build

cmake ..

make

echo "\nassembler working on assembly"
./Assembler ../test/assembly ../test/machinecode/machine
echo "\nassembler working on factorial"
./Assembler ../test/factorial ../test/machinecode/factorial_machine
echo "\nassembler working on combination"
./Assembler ../test/cnr ../test/machinecode/cnr_machine

cd ..

g++ -o old old.cpp
echo "\nassembler(Old) working on assembly"
./old ./test/assembly ./test/machinecode/old_machine
echo "\nassembler(Old) working on factorial"
./old ./test/factorial ./test/machinecode/old_factorial_machine
echo "\nassembler(Old) working on combination"
./old ./test/cnr ./test/machinecode/old_cnr_machine

rm old