#!/bin/bash 
rm -rf "./problem/"
mkdir "./problem/"
mkdir "./problem/in"
mkdir "./problem/out"

for i in {1..10}
do
    rm -f "./problem/in/input$i.txt"
    touch "./problem/in/input$i.txt"
    dimension=$((RANDOM % 10 + 1))
    echo "$dimension" >> "./problem/in/input$i.txt"
    total=1
    for ((j=1; j<=dimension; j++))
    do
        random_number=$((RANDOM % 5 + 1))
        total=$((total * random_number))
        echo -n "$random_number " >> "./problem/in/input$i.txt"
    done
    for ((j=1; j<=total; j++))
    do
        random_number=$((RANDOM % 100))
        echo -n "$random_number " >> "./problem/in/input$i.txt"
    done
    echo -e "" >> "./problem/in/input$i.txt"
    for ((j=1; j<=total; j++))
    do
        random_number=$((RANDOM % 100))
        echo -n "$random_number " >> "./problem/in/input$i.txt"
    done
    operation=$((RANDOM % 10 + 1))
    echo "$operation" >> "./problem/in/input$i.txt"
    for ((j=1; j<=operation; j++))
    do
        random_number=$((RANDOM % 100))
        if [ $random_number -le 30 ]
        then
            echo "T" >> "./problem/in/input$i.txt"
        elif [ $random_number -le 95 ]
        then
            random_number=$((RANDOM % 2 + 1))
            a=$((RANDOM % 1000000))
            b=$((RANDOM % 1000000))
            echo "L $random_number $a $b" >> "./problem/in/input$i.txt"
        else
            if [ $((total % 2)) -eq 0 ]
            then
                temp=$((total / 2))
                echo "R 2 2 $temp" >> "./problem/in/input$i.txt"
            else
                echo "R 1 $total" >> "./problem/in/input$i.txt"
            fi
        fi
    done
done

gcc 8.c -o 8.out

for i in {1..10}
do
    rm -f "./problem/out/output$i.txt"
    ./8.out < "./problem/in/input$i.txt" > "./problem/out/output$i.txt"
done
