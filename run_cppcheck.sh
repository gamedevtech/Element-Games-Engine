#!/bin/sh
cppcheck --enable=all --quiet --verbose Engine/ 2> cppcheck_results.txt
