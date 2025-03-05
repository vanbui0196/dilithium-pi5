#!/bin/bash

# Define the number of runs
num_runs=10000
output_file="sig_test_output.csv"

# Clear the output file if it exists
> $output_file

# Redirect all output to the file
{
    # Loop to run the command multiple times
    for ((i=1; i<=num_runs; i++))
    do
        ./sig_test
    done
} > $output_file 2>&1

echo "All output has been saved to $output_file"
