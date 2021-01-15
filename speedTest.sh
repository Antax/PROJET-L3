#!/bin/bash

if [ $# -ne 2 ] 
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions"
    exit 1
fi

if [ "--SAT" != $1 ] && [ "--DET" != $1 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions"
    exit 1
fi

if [ 0 -ge $2 ]
then
    echo "number of excutions must be strictely positive"
fi
echo $2

if [ "--SAT" = $1 ]
then 
    for i in $(seq $2)
    do
        ./Automaton --SAT
        minisat test.cnf test.out
    done
else
    for i in {0..10}
    do
        ./TestsAutomaton
    done
fi