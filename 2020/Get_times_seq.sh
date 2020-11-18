#!/bin/sh

make all

echo "### Seq Opt"
# shellcheck disable=SC2039
for i in {1..20}
do
    echo "Attempt $i"
    time ./image-seq copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done

echo "### Seq No Opt"
# shellcheck disable=SC2039
for i in {1..20}
do
    echo "Attempt $i"
    time ./image-seq-nop copy ./images_source ./images_destination
    rm -rf images_destination/*.bmp
done

exit 0
