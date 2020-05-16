#!/usr/bin/env bash

if ! [[ -x "$(command -v clang-format)" ]]; then
  echo 'Error: clang-format is NOT installed.' >&2
  exit 1
fi

clang-format --verbose -i --style=file include/impl/*cpp include/util/*cpp
clang-format --verbose -i --style=file include/*h
clang-format --verbose -i --style=file src/*cpp
clang-format --verbose -i --style=file test/*cpp

exit 0;
