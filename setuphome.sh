#!/bin/bash

# Comment
echo "Start"
sudo iptables --table nat --append POSTROUTING --out-interface enp2s0 -j MASQUERADE
# sudo iptables --table nat --append POSTROUTING --out-interface wlp3so -j MASQUERADE
sudo iptables --append FORWARD --in-interface enxd0397238b2a0 -j ACCEPT
sudo sh -c "echo 1 > /proc/sys/net/ipv4/ip_forward"
echo "Finish"
ssh -x root@192.168.7.2
