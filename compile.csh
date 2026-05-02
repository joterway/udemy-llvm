#!/usr/bin/csh
clang++ -o eva `llvm-config --cxxflags --ldflags --system-libs --libs core ` eva.cpp
./eva
lli ./out.ll

echo $?
