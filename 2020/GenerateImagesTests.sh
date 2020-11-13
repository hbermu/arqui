#!/bin/sh

./image-seq-test gauss ./images_source ./images_destination_gauss

./image-seq-test sobel ./images_source ./images_destination_sobel

exit 0
