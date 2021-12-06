#!/bin/bash

if [ "$#" -ne 1 ]; then
    echo "Usage: $0 <virtual interface name (example: vcan0)>"
    exit 
fi

echo "Sudo password is required in order to modprobe the needed modules and setup $1 interface."
sudo modprobe can
sudo modprobe can_raw
sudo modprobe vcan
sudo ip link add dev $1 type vcan
sudo ip link set up $1
ip link show $1
