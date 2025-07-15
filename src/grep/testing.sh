#!/bin/bash
test_grep () {
  COUNTER=$((COUNTER + 1))
  grep $@ > test1.txt
  ./s21_grep $@ > test2.txt
  diff -q test1.txt test2.txt > /dev/null
  if [ $? -eq 0 ]; then
      SUCCESS=$((SUCCESS + 1))
      echo "$COUNTER - [${GREEN}SUCCES${NC}] with $@"
  else
      FAIL=$((FAIL + 1))
      echo "$COUNTER - [${RED}FAIL${NC}] with $@"
      sleep 1
  fi
}

leak_test_grep () {
  COUNTER=$((COUNTER + 1))
  $VALGRIND $@ > /dev/null
  if [ ! -s $log ]; then
      SUCCESS=$((SUCCESS + 1))
      echo "$COUNTER - [${GREEN}SUCCES${NC}] with $@"
  else
      FAIL=$((FAIL + 1))
      echo "$COUNTER - [${RED}FAIL${NC}] with $@"
      sleep 1
  fi
}

COUNTER=0
SUCCESS=0
FAIL=0

RED='\033[0;31m'
GREEN='\033[0;32m'
NC='\033[0m'

log="test3.txt"
VALGRIND="valgrind -q --leak-check=full --log-file=$log ./s21_grep"

FLAGS="-i -v -e -c -l -n -h -s -o -lol"
FLAGS1="-n -s -v -i -h -c -l"
FLAGS2="-h -l -v -s -c -i -n"
FLAGS3="-i -v -c -l -n -h -s"
F_FLAG="-f"

NO_EXIST="no_exist.txt"
FILES="../examples/example0.txt ../examples/example1.txt ../examples/example2.txt ../examples/test_0_grep.txt ../examples/example"
FILES1="../examples/example1.txt ../examples/example ../examples/0.txt"
FILES_PATTERN="no_exist.txt ../examples/arg.txt"
PATTERNS="a de fa he e pro"
PATTERNS1="in it c Seg lan"

echo '\t''\t'"FUNCTIONAL TESTS"
echo "==============================================================="
sleep 0.5

for file in $FILES; do
    for pattern in $PATTERNS; do
        test_grep "$pattern $file"
        for flag in $FLAGS; do
            test_grep "$flag $pattern $file"
        done
    done    
done
for file_pattern in $FILES_PATTERN; do
    for file in $FILES; do
        test_grep "-f $file_pattern $file"
    done
done 
for file_pattern in $FILES_PATTERN; do
    for file in $FILES; do
        for flag in $FLAGS; do
            test_grep "$flag -f $file_pattern $file"
        done
    done
done 
for pattern in $PATTERNS; do
    for pattern1 in $PATTERNS1; do
        test_grep "-e $pattern -e $pattern1 $file"
    done
done 
for pattern in $PATTERNS; do 
    test_grep "$pattern $NO_EXIST"
done
for pattern in $PATTERNS; do
    for file in $FILES; do
        for file1 in $FILES1; do
            if [ $file != $file1 ]; then            
                test_grep "-o $pattern $file $file1"
            fi
        done
    done
done 
for flag3 in $FLAGS3; do
    for flag1 in $FLAGS1; do
        for flag2 in $FLAGS2; do
            for pattern in $PATTERNS; do
                for file in $FILES; do
                    for file1 in $FILES1; do
                        if [ $flag3 != $flag1 ] && [ $flag1 != $flag2 ] && [ $flag3 != $flag2 ] && [ $file != $file1 ]; then            
                                test_grep "$flag1 $flag2 $flag3 $pattern $file $file1"
                        fi
                    done    
                done
            done
        done
    done   
done

echo '\t''\t'"MEMORY LEAK TESTS"
echo "==============================================================="
sleep 0.5

for pattern in $PATTERNS; do
    for flag in $FLAGS; do     
        leak_test_grep "$flag $pattern $file"
    done
done    
for file_pattern in $FILES_PATTERN; do
    for file1 in $FILES1; do
        leak_test_grep "-f $file_pattern $file1"
    done
done


echo "ALL: $COUNTER"
echo "${GREEN}SUCCESS${NC} = $SUCCESS"
echo "${RED}FAILS${NC} = $FAIL"

rm -rf test1.txt test2.txt test3.txt