#!/bin/bash
set -e
echo "Running CPU test..."
./tests/test_cpu
echo "Running Mem test..."
./tests/test_mem
echo "All tests passed!"

