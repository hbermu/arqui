#!/bin/bash

make all

export OMP_NESTED=true
export OMP_MAX_ACTIVE_LEVELS=2

echo "### Par 1"
export OMP_NUM_THREADS=1
echo "#Copy"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Gauss"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par gauss ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Sobel"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par sobel ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#####################################################"
echo ""
echo ""

echo "### Par 2"
export OMP_NUM_THREADS=2
echo "#Copy"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Gauss"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par gauss ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Sobel"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par sobel ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#####################################################"
echo ""
echo ""

echo "### Par 16"
export OMP_NUM_THREADS=16
echo "#Copy"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Gauss"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par gauss ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Sobel"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-par sobel ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#####################################################"
echo ""
echo ""




exit 0
