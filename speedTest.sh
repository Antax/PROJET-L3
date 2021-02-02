#!/bin/bash

if [ $# -ne 2 ] && [ $# -ne 3 ] && [ $# -ne 4 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions maxLength (NB/rand)"
    exit 1
fi

if [ "--SAT" != $1 ] && [ "--DET" != $1 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions maxLength (NB/rand)"
    exit 1
fi

if [ 0 -ge $2 ]
then
    echo "number of executions must be strictely positive"
    exit 1
fi

if [ "--SAT" == $1 ] 
then
    if [ $# -eq 2 ] || [ 0 -ge $3 ]
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
            if [ $# -ne 4 ]
            then
                ./Automaton --SAT $j $i
            else
                if [ $4 == "rand" ]
                then
                    ./Automaton --SAT $j
                else
                    ./Automaton --SAT $j $[$i+$4]
                fi
            fi
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
        if [ $# -ne 3 ]
        then
            ./TestsAutomaton $i
        else
            if [ $3 == "rand" ]
            then
                ./TestsAutomaton
            else
                ./TestsAutomaton $[$i+$3]
            fi
        fi
    done
fi