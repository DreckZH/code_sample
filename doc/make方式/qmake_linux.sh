#!/bin/bash

qmake -o mmsclient.mak mmsclient.pro
make -f mmsclient.mak
