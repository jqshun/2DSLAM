How to Install
===
	sudo apt-key adv --keyserver keys.gnupg.net --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE || sudo apt-key adv --keyserver hkp://keyserver.ubuntu.com:80 --recv-key F6E65AC044F831AC80A06380C8B3A55A6F3EFCDE

```xml
In ubuntu 16.04:

sudo add-apt-repository "deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo xenial main" -u

In unbutu 18.04:

sudo add-apt-repository "deb http://realsense-hw-public.s3.amazonaws.com/Debian/apt-repo bionic main" -u

```

Install dependencies:
```xml
sudo apt-get install librealsense2-dkms

sudo apt-get install librealsense2-utils

sudo apt-get install librealsense2-dev

sudo apt-get install librealsense2-dbg
```

# **To check if success**
realsense-viewer           

sudo apt-get update

sudo apt-get upgrade

# **In catkin_ws/src

git clone https://github.com/IntelRealSense/realsense-ros.git

cd realsense-ros/

git checkout `git tag | sort -V | grep -P "^\d+\.\d+\.\d+" | tail -1`

cd ..

catkin_make

# **If there is API error**

Use gitkraken, File open repo, "realsense-ros", checkout to version 2.2.9

