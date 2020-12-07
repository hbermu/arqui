#!/bin/bash

make all

export OMP_NESTED=true
export OMP_MAX_ACTIVE_LEVELS=2

echo "### Par 4 static"
export OMP_SCHEDULE=static
export OMP_NUM_THREADS=4
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

echo "### Par 8 static"
export OMP_NUM_THREADS=8
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

echo "### Par 4 dynamic"
export OMP_SCHEDULE=dynamic
export OMP_NUM_THREADS=4
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

echo "### Par 8 dynamic"
export OMP_NUM_THREADS=8
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

echo "### Par 4 guided"
export OMP_SCHEDULE=guided
export OMP_NUM_THREADS=4
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

echo "### Par 8 guided"
export OMP_NUM_THREADS=8
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
