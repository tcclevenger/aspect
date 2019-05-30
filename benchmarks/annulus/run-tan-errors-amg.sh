#!/bin/bash

for ref in 1 2 3 4 5 6; do
  cp annulus-amg.prm current.prm
  echo "subsection Mesh refinement" >> current.prm
  echo "  set Initial global refinement = $ref" >> current.prm
  echo "end" >> current.prm

  ./aspect current.prm | grep "Errors"
done
