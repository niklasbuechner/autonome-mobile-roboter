pip install Pygments
@echo off
SET RESULT_DIR=..\result
SET OUTPUT_DIR=temp
SET FILE=thesis
cd tex
mkdir %RESULT_DIR%
mkdir %OUTPUT_DIR%
pdflatex  -output-directory=%OUTPUT_DIR% -synctex=1 -shell-escape  %FILE%.tex
biber %OUTPUT_DIR%\%FILE%
makeindex %OUTPUT_DIR%\%FILE%.idx
makeglossaries %OUTPUT_DIR%\%FILE%
pdflatex -output-directory=%OUTPUT_DIR% -synctex=1 -shell-escape %FILE%.tex
pdflatex -output-directory=%OUTPUT_DIR% -synctex=2 -shell-escape %FILE%.tex
copy %OUTPUT_DIR%\%FILE%.pdf %RESULT_DIR%\%FILE%.pdf
cd ..
