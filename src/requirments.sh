#!/bin/bash

#apt-get -y --force-yes update
#apt-get -y --force-yes upgrade
#update-manager -c -d
 
apt-get -y --force-yes install curl cmake golang-go libglib2.0-dev ffmpeg
libopencv-dev cmake libcurl4-openssl-dev libboost-dev libboost-filesystem-dev
 libboost-system-dev libboost-log-dev libopenblas-dev python3-tk mysql-common mysql-client

add-apt-repository ppa:graphics-drivers/ppa
apt-get install nvidia-driver-415
