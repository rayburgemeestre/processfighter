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


