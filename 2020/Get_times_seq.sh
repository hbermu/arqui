#!/bin/bash

make all

echo "### Seq Opt"
echo "#Copy"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-seq copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Gauss"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-seq gauss ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Sobel"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-seq sobel ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done

echo "### Seq No Opt"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-seq-nop copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Gauss"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-seq-nop gauss ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done
echo "#Sobel"
for i in {1..20}
do
    echo "Attempt ${i}"
    time ./image-seq-nop sobel ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done

exit 0
