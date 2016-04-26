# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: clean build  run

OS:=$(shell uname)


ifeq ($(OS), Darwin)
	NUMCPUS:=`sysctl -n hw.ncpu`
else
	NUMCPUS:=`grep -c '^processor' /proc/cpuinfo`
endif

build:
	git submodule update --init --recursive
	cd build/ && cmake ../src && make -j ${NUMCPUS}

clean:
	rm -rf build/*

run:
ifeq (${OS}, Darwin)
	cd bin/ && ./MTE7102.app/Contents/MacOS/MTE7102
else ifeq (${OS}, Linux)
	cd bin/ && ./MTE7102
else
	@echo "You are running this Makefile from Windows? Seriously!?"
endif
