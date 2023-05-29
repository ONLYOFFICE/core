############################################################
# Dockerfile to build V8  container images
# Based on Ubuntu
############################################################
# Set the base image to Ubuntu
FROM ubuntu:18.04
# File Author / Maintainer
MAINTAINER onlyoffice.com
################## BEGIN INSTALLATION ######################
# Update Image
RUN apt-get update
RUN apt-get install -y sudo
RUN apt-get install -y apt-utils
RUN useradd -m docker && echo "docker:docker" | chpasswd && adduser docker sudo
RUN echo "docker ALL=(ALL) NOPASSWD:ALL" >> /etc/sudoers
user docker
# Update depedency of V8
RUN sudo apt-get install -y \
                lsb-core \
                git \
                python \
                lbzip2 \
                curl    \
                wget    \
                xz-utils \
                zip \
                python2.7 \
                python3 \
                autoconf \
                build-essential \
                cmake \
                pkg-config \
                libc6-i386 \
                gcc-multilib \
                software-properties-common
WORKDIR /home/docker
RUN mkdir v8
WORKDIR /home/docker/v8
COPY ./build.py ./build.py
RUN sudo add-apt-repository -y  ppa:deadsnakes/ppa
RUN sudo apt install -y python3.8
RUN sudo update-alternatives --install /usr/bin/python python /usr/bin/python3.8 1000
RUN sudo update-alternatives --install /usr/bin/python3 python3 /usr/bin/python3.8 1000
RUN python ./build.py
RUN zip -r ./build.zip ./build/*
RUN ls -al /home/docker/v8/build.zip
#End of docker Command