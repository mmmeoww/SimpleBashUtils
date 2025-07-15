#!/bin/bash

FLAGS="-b -e -n -s -t -E -T --number --number-nonblank --squeeze-blank"
BIG_FLAG="-benstET"
FILES="../examples/all_sym_160.txt ../examples/example0.txt ../examples/example1.txt ../examples/example2.txt ../examples/example ../examples/test_0_grep.txt ../examples/0.txt"

SUCCESS=0
FAIL=0

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0;0m'

if [ ! -f ./s21_cat ]; then
    echo "./s21_cat is not compiled!"
    echo "please run: make s21_cat"
else
    echo "./s21_cat with multiple files:"
    sleep 1
    for flag in $FLAGS; do
        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo "./s21_cat $flag ../examples/*:"
        echo -n " TEST - "
        cat $flag ../examples/* > test1.txt
        ./s21_cat $flag ../examples/* > test2.txt
        diff -q test1.txt test2.txt > /dev/null
        if [ $? -eq 0 ]; then
            SUCCESS=$((SUCCESS + 1))
            echo "[${GREEN}PASS${NC}]"
        else
            FAIL=$((FAIL + 1))
            echo "[${RED}FAIL${NC}]"
        fi
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.2
    done
    echo

    echo "./s21_cat with non-existing flags:"
    sleep 1
    for file in $FILES; do
        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo "./s21_cat -x $file:"
        cat -x $file > test1.txt
        ./s21_cat -x $file > test2.txt
        diff -q test1.txt test2.txt > /dev/null
        if [ $? -eq 0 ]; then
            SUCCESS=$((SUCCESS + 1))
            echo " TEST - [${GREEN}PASS${NC}]"
        else
            FAIL=$((FAIL + 1))
            echo " TEST - [${RED}FAIL${NC}]"
        fi
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.2
    done
    echo

    echo "./s21_cat with non-existing flags and files:"
    sleep 1
    echo "~~~~~~~~~~~~~~~~~~~~~"
    echo "./s21_cat -x ne.txt:"
    cat -x ne.txt > test1.txt
    ./s21_cat -x ne.txt > test2.txt
    diff -q test1.txt test2.txt > /dev/null
    if [ $? -eq 0 ]; then
        SUCCESS=$((SUCCESS + 1))
        echo " TEST - [${GREEN}PASS${NC}]"
    else
        FAIL=$((FAIL + 1))
        echo " TEST - [${RED}FAIL${NC}]"
    fi
    echo "~~~~~~~~~~~~~~~~~~~~~"
    sleep 0.2

    for file in $FILES; do
        echo "./s21_cat with $file file:"
        sleep 1

        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo "./s21_cat $file:"
        echo -n " TEST - "
        cat $file > test1.txt
        ./s21_cat $file > test2.txt
        diff -q test1.txt test2.txt > /dev/null
        if [ $? -eq 0 ]; then
            SUCCESS=$((SUCCESS + 1))
            echo "[${GREEN}PASS${NC}]"
        else
            FAIL=$((FAIL + 1))
            echo "[${RED}FAIL${NC}]"
        fi
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.2

        for flag in $FLAGS; do
            echo "~~~~~~~~~~~~~~~~~~~~~"
            echo "./s21_cat $flag $file:"
            echo -n " TEST - "
            cat $flag $file > test1.txt
            ./s21_cat $flag $file > test2.txt
            diff -q test1.txt test2.txt > /dev/null
            if [ $? -eq 0 ]; then
                SUCCESS=$((SUCCESS + 1))
                echo "[${GREEN}PASS${NC}]"
            else
                FAIL=$((FAIL + 1))
                echo "[${RED}FAIL${NC}]"
            fi
            echo "~~~~~~~~~~~~~~~~~~~~~"
            sleep 0.2
        done
    done

    for file in $FILES; do
        echo "./s21_cat $BIG_FLAG $file:"
        sleep 1
        echo "~~~~~~~~~~~~~~~~~~~~~"
        echo -n " TEST - "
        cat $BIG_FLAG $file > test1.txt
        ./s21_cat $BIG_FLAG $file > test2.txt
        diff -q test1.txt test2.txt > /dev/null
        if [ $? -eq 0 ]; then
            SUCCESS=$((SUCCESS + 1))
            echo "[${GREEN}PASS${NC}]"
        else
            FAIL=$((FAIL + 1))
            echo "[${RED}FAIL${NC}]"
        fi
        echo "~~~~~~~~~~~~~~~~~~~~~"
        sleep 0.2
    done

    echo "./s21_cat $BIG_FLAG ../examples/*:"
    sleep 1
    echo "~~~~~~~~~~~~~~~~~~~~~"
    echo -n " TEST - "
    cat $BIG_FLAG ../examples/* > test1.txt
    ./s21_cat $BIG_FLAG ../examples/* > test2.txt
    diff -q test1.txt test2.txt > /dev/null
    if [ $? -eq 0 ]; then
        SUCCESS=$((SUCCESS + 1))
        echo "[${GREEN}PASS${NC}]"
    else
        FAIL=$((FAIL + 1))
        echo "[${RED}FAIL${NC}]"
    fi
    echo "~~~~~~~~~~~~~~~~~~~~~"
    sleep 0.2
    echo "${GREEN}SUCCESS${NC} = $SUCCESS"
    echo "${RED}FAILS${NC} = $FAIL"
fi

rm -rf test1.txt test2.txt test3.txt