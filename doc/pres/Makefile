# -*- coding: UTF-8 -*-
# vim: autoindent expandtab tabstop=4 sw=4 sts=4 filetype=make

.PHONY: all

build:
	prez

watch:
	fswatch -o slides -r --include 'slides/.*' --include 'css/.*' --include 'images/.*' --exclude '.*' | (while read; do prez; done)

all: build
