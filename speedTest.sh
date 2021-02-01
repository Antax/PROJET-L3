#!/bin/bash

if [ $# -ne 2 ] && [ $# -ne 3 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions maxLength"
    exit 1
fi

if [ "--SAT" != $1 ] && [ "--DET" != $1 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions maxLength"
    exit 1
fi

if [ 0 -ge $2 ]
then
    echo "number of executions must be strictely positive"
    exit 1
fi

if [ "--SAT" == $1 ] 
then
    if [ $# -ne 3 ] || [ 0 -ge $3 ]
    then
        echo "max length must be strictely positive"
        exit 1
    fi
fi

if [ "--SAT" = $1 ]
then 
    for i in $(seq $2)
    do
        for j in $(seq $3)
        do
            ./Automaton --SAT $j
            minisat test.cnf test.out &>/dev/null
            line=$(head -n 1 test.out)
            if [ "SAT" == $line ]
            then
            echo $j
                break
            fi
            if [ $j == $3 ]
            then
            echo 'Automate is Include'
            fi
        done
    done
else
    for i in $(seq $2)
    do
        ./TestsAutomaton
    done
fi