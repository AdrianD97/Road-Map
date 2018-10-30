#!/bin/bash

NO_TASKS=5
NO_TESTS=8
EXEC=roadMap
MAX_BONUS=10
TEST_POINTS=(2 2 2 2 2 2 2 2 2)
TIMEOUT_TIME=(1 1 1 1 1 2 3 4 4)
VALGRIND_TIMEOUT_TIME=(14 14 14 14 14 50 50 80 80)
TOTAL=0

ERRORS_NUMBER=0
CODING_STYLE_BONUS=0
CURRENT_DIRECTORY=`pwd`
ONE=1
TWO=2
MESSAGE_SIZE=12
PASSED_TESTS_GRADE=0

make build

timeout_test()
{
	test_no=$1
    input_path=$2

    # Get the input and ref files for the current test
	timeout=${TIMEOUT_TIME[$test_no]}
	(time timeout $timeout ./$EXEC <$input_path) 2>time.err
	TEST_RESULT=$?
}

valgrind_test()
{
    test_no=$1
    input_path=$2

	# Get the input and ref files for the current test
	timeout=${VALGRIND_TIMEOUT_TIME[$test_no]}

	(time timeout $timeout valgrind --leak-check=full --error-exitcode=1 -q ./$EXEC <$input_path) 2>time.err
	TEST_RESULT=$?
}


# Check a single test and add the result to the total score. This function
# should _always_ receive a test number as the first and only argument
check_test()
{
    test_no=$1
    input_path="input/test"$test_no".in"

    #check valgrind first
    check_valgrind_test $1

    if [ "$TEST_RESULT" != "0" ]; then
         echo "Test: $test_no ...................... FAILED BECAUSE OF VALGRIND"
         rm -f result.out
         rm -f time.err
         return
    fi

	timeout_test $test_no $input_path

    # Run the test, also consider the timeout
    if [ "$TEST_RESULT" != "0" ]; then
        echo "TIMEOUT [$timeout s]"
        rm -f task*.out
        rm -f time.err
		return
	fi

    for ((j=1;j<=$NO_TASKS;j++))
    do
        out_path="task"$j".out"
        ref_path="ref/test"$test_no"/task"$j".out"

        echo -n "Test: $test_no task $j ...................... "

        # Check the result
        diff -bBZ -i $ref_path $out_path 2>&1 1> my_diff
        if test $? -eq 0; then
            echo "PASS [${TEST_POINTS[$test_no]}p]"
    		#if [ -f time.err  ]; then
    			#cat time.err
    		#fi
            TOTAL=$(echo $TOTAL + ${TEST_POINTS[$test_no]} | bc)
            PASSED_TESTS_GRADE=$(($PASSED_TESTS_GRADE+1));
        else
            echo "FAILED"
            echo "Diff result:"
            cat my_diff | tail -n 10
            #cat time.err
        fi
    done

    # Clean up
    rm -f my_diff
    rm -f task*.out
    rm -f time.err
}


check_valgrind_test()
{
    test_no=$1
    input_path="input/test"$test_no".in"

    #echo
    echo -n "Test: $test_no ...................... "

    valgrind_test $test_no $input_path

    if [ "$TEST_RESULT" != "0" ]; then
        echo "DETECTED MEMORY LEAKS"
        #tail -n 10 time.err
        rm -f result.out
        rm -f time.err
        return
    else
        echo "NO MEMORY LEAKS"
        rm -f result.out
        rm -f time.err
        return
    fi
}

function checkBonus
{
    printf '%*s\n' "${COLUMNS:-$(($(tput cols) - $ONE))}" '' | tr ' ' -
    echo "" > checkstyle.txt

    echo -ne "Coding style Errors:\n"
    for entry in *.{cpp,hpp,h};
    do
        if [[ -f $entry ]]; then
            ./cpplint.py "$entry" > checkstyle.txt
            YOUR_BONUS=`cat checkstyle.txt | grep "Total errors found: "| tr -dc '0-9'`
            ERRORS_NUMBER=$(( $ERRORS_NUMBER + $YOUR_BONUS ))
        fi
    done

    if [[ $ERRORS_NUMBER != "0" ]]; then
        printf '\nCoding style check: %*sFAIL' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE))}" '' | tr ' ' -
        echo -ne "\n\t\tYou have a total of $ERRORS_NUMBER coding style errors.\n"

    else
        echo -ne "\n\tNone?\n\t\tWow, next level!"
        printf '\n\nCoding style check: %*sOK' "${COLUMNS:-$(($(tput cols) - $MESSAGE_SIZE + $TWO ))}" '' | tr ' ' -

        CODING_STYLE_BONUS=$(echo "scale=1; $TOTAL / 8" | bc -l)

        # CODING_STYLE_BONUS=$(($CODING_STYLE_BONUS))
    fi
    rm -f checkstyle.txt
}

function printBonus
{
    if [[ $ERRORS_NUMBER -gt 0 ]]; then
    printf '\n%*s' "${COLUMNS:-$(tput cols)}" '' | tr ' ' -
        echo -ne  "\n\tAnd you almost did it!\n"
    else
        echo -ne "\n\tDamn!\n"
    fi
    echo -ne "\n\t\tYou get $CODING_STYLE_BONUS/$MAX_BONUS for coding style.\n\n"
}

# Check if the the exec exist
if test ! -x $EXEC; then
    echo "Execf file missing"
    exit 1
fi

for ((i=1;i<=$NO_TESTS;i++))
do
	check_test $i
done

# And the restul is ...
echo "TOTAL: $TOTAL/80"
echo ""

checkBonus
printBonus

# mkdir resources
# cp archive.zip resources/archive.zip
# cd resources
# unzip archive.zip
# cd ..
# checkBonus resources
# printBonus
# rm -rf resources
