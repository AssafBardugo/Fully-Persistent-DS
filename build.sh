#!/bin/bash

mkdir -p build

# Defaults:
action="all"
memcheck="false"

for arg in "$@"
do
    case $arg in
        --memcheck)
            memcheck="true"
            ;;
        --test=*)
            action="${arg#*=}"
            ;;
        --all)
            action="all"
            ;;
        --help)
            echo "Usage: $0 [--test=<test_name>|--all] [--memcheck]"
            echo ""
            echo "          --test=<test_name>    Run specific test"
            echo "          --all                 Run all tests together (default)"
            echo "          --memcheck            Run Valgrind memory check on selected tests"
            exit 0
            ;;
        *)
            echo "Unknown argument: $arg"
            echo "Usage: $0 [--test=<test_name>|--all] [--memcheck]"
            exit 1
            ;;
    esac
done

make clean
echo "Building tests..."
make test

# Run the chosen action
if [ "$memcheck" == "true" ]; then
    echo "Running Valgrind on selected tests..."
    valgrind --leak-check=full --error-exitcode=1 ./test $action || exit 1
else
    echo "Running selected tests..."
    ./test $action
fi
