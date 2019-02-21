#!/bin/bash

# credits to AI :D
TEST_DIR=test
TEST_LIST=$TEST_DIR/tests
REF_DIR=$TEST_DIR/ref
TEST_FILE=input
VARS_OUT=output
TREE_OUT=arbore

# Compile student homework
# comment this if you use Java or Haskell - this means that the executable should be generated before running this script
make build

# Run tests
score=0
total_score=0
while read test_case
do
	# Clean latest output
	rm $TEST_FILE &> /dev/null
	rm $VARS_OUT &> /dev/null
	rm $TREE_OUT &> /dev/null

	# Parse the components of the line
	test_name="$(echo $test_case | cut -d' ' -f 1)"
	tree_points="$(echo $test_case | cut -d' ' -f 2)"
	vars_points="$(echo $test_case | cut -d' ' -f 3)"
	input="$(echo $test_case | cut -d' ' -f 4)"
	vars_ref="$(echo $test_case | cut -d' ' -f 5)"
	tree_ref="$(echo $test_case | cut -d' ' -f 6)"

	# Create a link to the current test file
	ln -s $TEST_DIR/in/$input $TEST_FILE

	# Run the student homework
	make -s run &> /dev/null

	# Update scores
	total_score=$(($total_score + $tree_points + $vars_points))

	# Compare parsing results
	cmp -s $TREE_OUT $REF_DIR/$tree_ref

	if [ $? -eq 0 ]
	then
		echo "Test $test_name [PARSE] PASSED"; score=$(($score + $tree_points));
	else
		echo "Test $test_name [PARSE] FAILED"; echo "official output -> "$REF_DIR/$tree_ref
	fi

	# Compare interpreting results
	cmp -s $VARS_OUT $REF_DIR/$vars_ref

	if [ $? -eq 0 ]
	then
		echo "Test $test_name [INTERPRET] PASSED"; score=$(($score + $vars_points));
	else
		echo "Test $test_name [INTERPRET] FAILED"; echo "official output -> "$REF_DIR/$vars_ref
	fi
done < $TEST_LIST

rm $TEST_FILE &> /dev/null
rm $VARS_OUT &> /dev/null
rm $TREE_OUT &> /dev/null
echo "Score: " $score " out of $total_score"
