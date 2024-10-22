#!/bin/bash

source_files=`ls ./src/*.cpp`

g++ -o main "$source_files"
