#!/bin/bash

function check_installed
{
    typeset tool=$1
    echo -n "checking whether '$tool' is installed.. "
    if ! [[ $(which $tool) ]]; then 
        echo "[ no (aborting) ]"
        exit 1
    else
        echo "[ yes ]"
    fi
}

check_installed arp
check_installed sed

# Want to do something like arp -an | sed 's/.*(\(.*\)).*/\1/'

check_installed git
check_installed cmake

git submodule update --init --recursive

(
cd libs/tiny-process-library
mkdir build
cd build
cmake ..
make
#./examples
)

if ! [[ -f "./libs/tiny-process-library/build/libtiny-process-library.a" ]]; then
    echo "building tiny-process-library didn't produce expected libtiny-process-library.a"
    exit 1
fi
echo bootstrap succesful
