#! /bin/csh -f
set TEST_HOME   = ${PWD}/tests
set source_file = my-unzip.c
set binary_file = my-unzip
set bin_dir     = ${TEST_HOME}/bin
set test_dir    = ${TEST_HOME}/tests-my-unzip
${bin_dir}/generic-tester.py -s $source_file -b $binary_file -t $test_dir $argv[*]
