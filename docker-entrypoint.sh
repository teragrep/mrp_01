#!/bin/bash
cd /code || exit 1;
autoreconf -fvi
bash configure
make
make check
make install DESTDIR=/code/buildroot