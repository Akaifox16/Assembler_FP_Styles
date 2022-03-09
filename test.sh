#!/bin/zsh

cd build

cmake ..

make

echo "\n--------------------------"
echo "\n        Assembler         "
echo "\n--------------------------\n"

echo "\nWorking on assembly"
./Assembler ../test/assembly ../test/machinecode/machine
echo "\nWorking on factorial"
./Assembler ../test/factorial ../test/machinecode/factorial_machine
echo "\nWorking on combination"
./Assembler ../test/cnr ../test/machinecode/cnr_machine

cd ..

echo "\n--------------------------"
echo "\n      Assembler(Old)      "
echo "\n--------------------------\n"

g++ -o old old.cpp
echo "\nWorking on assembly"
./old ./test/assembly ./test/machinecode/old_machine
echo "\nWorking on factorial"
./old ./test/factorial ./test/machinecode/old_factorial_machine
echo "\nWorking on combination"
./old ./test/cnr ./test/machinecode/old_cnr_machine

rm old

cd ./test/machinecode
echo "\n--------------------------"
echo "\n          TEST            "
echo "\n--------------------------\n"
g++ -o compare compare.cpp

echo "\nTesting on machine-code"
./compare machine old_machine

echo "\nTesting on factorial-machine-code"
./compare factorial_machine old_factorial_machine

echo "\nTesting on Combination-machine-code"
./compare cnr_machine old_cnr_machine

echo "\n--------------------------"
echo "\n         FINISH           "
echo "\n--------------------------\n"

rm compare