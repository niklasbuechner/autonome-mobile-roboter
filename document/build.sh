#!/bin/zsh

docker run --rm -v "$PWD/latex:/host-mounted" amr-latex:latest && open latex/tex/temp/thesis.pdf