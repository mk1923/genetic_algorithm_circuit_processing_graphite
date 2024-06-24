#!/bin/bash

## redirect to root directory if in build directory
#cd ..

# check if environment exists and create if not
if conda env list | grep -q 'graph-vector-env'; then
    echo "Environment 'graph-vector-env' already exists. Skipping environment creation."
else
    conda env create -f environment.yml
fi

# activate environment
source activate graph-vector-env

# install package
pip install -e .

# run script
run-script
