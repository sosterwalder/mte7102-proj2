# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: build clean run

OS=`uname`


ifeq ($(OS), Darwin)
	NUMCPUS=`sysctl -n hw.ncpu` + 1
else
	NUMCPUS=`grep -c '^processor' /proc/cpuinfo` + 1
endif

build:
	git submodule update --init --recursive
	cd build/ && cmake ../src && make -j ${$NUMCPUS}

clean:
	rm -rf build/*

run:
	cd bin/ && ./MTE7102
