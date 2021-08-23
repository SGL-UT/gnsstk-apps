#!/bin/bash

# Note: Assumes dependencies are passed in as arguments separated by spaces

for dependency in ${@}; do
  echo "Adding external dependency: ${dependency}"
  echo "find_package( ${dependency^^} CONFIG )" >>ExtLinkage.cmake
done
