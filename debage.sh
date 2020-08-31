#!/bin/sh
tar -xf circlepackings_1.0_bin_${1}.tar.gz -C circlepackings/usr/bin
strip circlepackings/usr/bin/circlepackings
cp control.${1} circlepackings/DEBIAN/control
cd circlepackings
chown -R root:root ./
chmod 755 -R usr/
chmod 755 DEBIAN/
find usr -type f -exec md5sum {} \; > DEBIAN/md5sums
find usr -type f -exec chmod 644 {} \;
chmod 755 usr/bin/circlepackings
chmod 644 DEBIAN/md5sums
cd ..
dpkg -b circlepackings/ circlepackings_1.0_${1}.deb
lintian circlepackings_1.0_${1}.deb
