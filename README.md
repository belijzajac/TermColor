# TermColor

![TermColor-preview](/img/TermColor_preview)

[YouTube link](https://youtu.be/QqSbcSTW530)

## About

A simple program written in modern C++17, Qt5 framework, and OpenCV library for dynamically generating color-schemes for terminal emulators. Main functions include:
* It uses the K-means clustering algorithm to detect dominant colors in an image
* It then brightens the latter colors to obtain intense colors
* It's fast (thanks to C++)

### Supported terminal emulators:

* Konsole (KDE)
* Xfce4 Terminal (Xfce4)
* LXTerminal (LXDE)

## Dependencies

* OpenCV >= 3.2.0
* Qt5 >= 5.11.3
* CMake >= 3.13
* g++ (any C++17 compatible version)

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
wget https://github.com/opencv/opencv/archive/3.2.0.zip -O opencv-3.2.0.zip

# Extract archive
unzip opencv-3.2.0.zip && rm opencv-3.2.0.zip
cd opencv-3.2.0 && mkdir build && cd build

# Build OpenCV
cmake -D CMAKE_BUILD_TYPE=RELEASE -D CMAKE_INSTALL_PREFIX=/usr/local ..
make -j $(nproc)
make install
```

In case you're on Debian (netinst) and you didn't install the `libopencv-dev` from the `sid` branch, there's a great chance that you'll also need the `zlib1g-dev` package in order to build TermColor

## Building

``` Shell
cd TermColor
mkdir build && cd build
cmake ..
make all
```

## Running

``` Shell
cd src
./TermColor
```

## License

This project is available under the MIT License. See the LICENSE file for more info.
