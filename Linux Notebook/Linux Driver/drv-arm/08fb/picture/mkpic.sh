#! /bin/bash

mkpic pic01/3.jpg 1image
mkpic pic01/4.jpg 2image
mkpic pic01/8.jpg 3image
mkpic pic01/11.jpg 4image
mkpic pic01/17.jpg 5image
mkpic pic01/19.jpg 6image

cat 2image >> 1image
cat 3image >> 1image
cat 4image >> 1image
cat 5image >> 1image
cat 6image >> 1image

mv 1image allimg.bin
rm -f *image

