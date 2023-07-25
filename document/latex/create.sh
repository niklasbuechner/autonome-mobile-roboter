#!/bin/sh
RESULT_DIR="../result"
OUTPUT_DIR="../temp"
FILE="thesis"
cd tex
mkdir $RESULT_DIR
mkdir $OUTPUT_DIR
cp literatur.bib $OUTPUT_DIR
pdflatex --shell-escape -output-directory="$OUTPUT_DIR" -synctex=1 -interaction=nonstopmode ${FILE}.tex
cd $OUTPUT_DIR
biber ${FILE}
makeindex ${FILE}.idx
makeglossaries ${FILE}
cd ../tex
pdflatex --shell-escape -output-directory="$OUTPUT_DIR" -synctex=1 -interaction=nonstopmode ${FILE}.tex
pdflatex --shell-escape -output-directory="$OUTPUT_DIR" -synctex=2 -interaction=nonstopmode ${FILE}.tex
cat $OUTPUT_DIR/${FILE}.pdf > $RESULT_DIR/${FILE}.pdf
cd ..
