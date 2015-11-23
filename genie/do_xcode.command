#!/bin/bash
BASEDIR=$(dirname $0)
cd $BASEDIR

clear
../../mix/vendor/bx/tools/bin/darwin/genie --xcode=tvos xcode7
../../mix/vendor/bx/tools/bin/darwin/genie --xcode=ios xcode7
../../mix/vendor/bx/tools/bin/darwin/genie --xcode=osx xcode7