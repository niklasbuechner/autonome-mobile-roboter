#!/bin/zsh

docker run --rm -v "$PWD/latex:/host-mounted" amr-latex:latest && open latex/result/thesis.pdf