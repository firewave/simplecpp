#!/bin/sh

output=$(./simplecpp simplecpp.cpp -e -f 2>&1)
ec=$?
errors=$(echo "$output" | grep -v 'Header not found: <')
if [ $ec -ne 0 ]; then
  # only fail if got errors which do not refer to missing system includes
  if [ ! -z "$errors" ]; then
    exit $ec
  fi
fi

gcc_ver=$(gcc -dumpversion)
./simplecpp simplecpp.cpp -e -f -D__GNUC__ -D__STDC__ -D__STDC_HOSTED__ -D__CHAR_BIT__=8 -I"/usr/include" -I"/usr/include/linux" -I"/usr/include/c++/$gcc_ver" -I"/usr/include/c++/$gcc_ver/x86_64-pc-linux-gnu" -I"/usr/lib64/gcc/x86_64-pc-linux-gnu/$gcc_ver/include"