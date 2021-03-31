#!/bin/bash

#Possiblité : 
# ./speedTest.sh  --SAT nbExec nbStates maxLength (rand || nb)
# ./speedTest.sh --DET nbExec nbStates (rand || nb)
# nb : for srand

if [ $# -ne 3 ] && [ $# -ne 4 ] && [ $# -ne 5 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions numberOfStates (maxLength (NB/rand))"
    exit 1
fi

if [ "--SAT" != $1 ] && [ "--DET" != $1 ]
then
    echo "Usage : ./speedTest.sh (--SAT/--DET) numberOfExecutions numberOfStates (maxLength (NB/rand))"
    exit 1
fi

#number of executions >0
if [ 0 -ge $2 ]
then
    echo "number of executions must be strictely positive"
    exit 1
fi

#number of states >0
if [ 0 -ge $3 ]
then
    echo "number of states must be strictely positive"
    exit 1
fi

if [ "--SAT" == $1 ] 
then
    #max length >0
    if [ $# -le 3 ] || [ 0 -ge $4 ]
    then
        echo "max length must be strictely positive"
        exit 1
    fi
fi

if [ "--SAT" = $1 ]
then 
    for i in $(seq $2) #number of executions -> to the random seed
    do
        for j in $(seq $4) #max length
        do
            if [ $# -ne 5 ]
            then
                ./Automaton --SAT $j $3 $i  #$i = seed
            else
                if [ $5 == "rand" ]   #$j = word's length
                then                  #$3 = number of state of A2
                    ./Automaton --SAT $j $3
                else
                    ./Automaton --SAT $j $3 $[$i+$5]        #$[$i+$5] = seed
                fi
            fi
            minisat Automaton.cnf Automaton.out &>/dev/null
            line=$(head -n 1 Automaton.out)
            if [ "SAT" == $line ]
            then
            echo $j
                break
            fi
            if [ $j == $4 ] #if maxlength has been tested
            then
            echo 'Automate is Included'
            fi
        done
    done
else #--DET
    for i in $(seq $2) #number of exectutions -> to the random seed
    do
        if [ $# -ne 4 ]
        then
            ./TestsAutomaton $3 $i
        else
            if [ $4 == "rand" ]
            then
                ./TestsAutomaton $3 
            else
                ./TestsAutomaton $3 $[$i+$4]
            fi
        fi
    done
fi