# TermColor

[![Build Status](https://travis-ci.org/belijzajac/TermColor.svg?branch=master)](https://travis-ci.org/belijzajac/TermColor)
[![Codacy Badge](https://api.codacy.com/project/badge/Grade/05fe181231ee449f98c7ad8bb49c8e62)](https://www.codacy.com/manual/belijzajac/TermColor?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=belijzajac/TermColor&amp;utm_campaign=Badge_Grade)
[![License](https://img.shields.io/badge/license-MIT-blue.svg?style=flat)](LICENSE)

![TermColor-preview](/img/TermColor_preview)

[YouTube link](https://youtu.be/QqSbcSTW530)

## About

A simple program written in modern C++17, Qt5 framework, and OpenCV library for dynamically generating color-schemes for terminal emulators. Main functions include:
*   It uses the K-means clustering algorithm to detect dominant colors in an image
*   It then brightens the latter colors to obtain intense colors
*   It's fast (thanks to C++)

### Supported terminal emulators

*   Konsole (KDE)
*   Xfce4 Terminal (Xfce4)
*   LXTerminal (LXDE)

[Status](STATUS.md)

## Dependencies

*   OpenCV >= 3.2.0
*   Qt5 >= 5.9.5
*   CMake >= 3.13
*   g++ (preferably version 9.2.1)

For Debian (stable/__testing__/__sid__), the following packages should be enough:

```bash
libopencv-dev *
qt5-default
cmake
g++
```

`*` - read below on how to properly obtain the OpenCV library

## Obtaining OpenCV

### Option 1: For Debian users

Considering you're either on Debian stable or testing branches, switch to Debian __sid__ branch by doing the following (as root):

```bash
nano /etc/apt/sources.list
```

Change your `/etc/apt/sources.list` file to point to "sid" by having its content similar to this:

```bash
deb http://ftp.lt.debian.org/debian/ sid main contrib non-free
deb-src http://ftp.lt.debian.org/debian/ sid main contrib non-free
```

Save it and exit. Now do the following:

```bash
apt clean && apt update
apt install libopencv-dev
```

Once everything is done installing, change back your `/etc/apt/sources.list` file and do `apt clean && apt update`

### Option 2: Building from source

Copy-paste the following commands as root:

```bash
# Required packages
apt install unzip wget cmake g++

# Download OpenCV (you can as well execute wget from your ~/Downloads foler)
wget https://github.com/opencv/opencv/archive/3.4.2.zip -O opencv-3.4.2.zip

# Extract archive
unzip opencv-3.4.2.zip && rm opencv-3.4.2.zip
cd opencv-3.4.2 && mkdir build && cd build

# Build OpenCV
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j $(nproc)
make install
```

In case you're on Debian (netinst) and you didn't install the `libopencv-dev` from the `sid` branch, there's a great chance that you'll also need the `zlib1g-dev` package in order to build TermColor

## Ubuntu

The project builds successfully on Ubuntu 19.04, however, for the g++ (the C++ compiler), install the package `g++-9`. See the [Issue #9](https://github.com/belijzajac/TermColor/issues/9) for a detailed workaround

## Arch Linux

As one person has pointed out, you need `opencv3-opt` from the AUR in order to build TermColor properly

## Building

``` Shell
git clone https://github.com/belijzajac/TermColor.git
cd TermColor
git submodule update --init --recursive
mkdir build && cd build
cmake ..
make TermColor
```

## Running

``` Shell
cd src
./TermColor
```

## License

This project is available under the MIT License. See the LICENSE file for more info.
