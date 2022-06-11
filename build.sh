#!/bin/bash

git submodule update --init

cmake -S . -B build/ -G "$1" -DBUILD_UNIT_TESTS=OFF -DBUILD_EXTRAS=OFF -DBUILD_BULLET2_DEMOS=OFF -DASSIMP_BUILD_TESTS=OFF -DASSIMP_INSTALL=OFF -DBUILD_SHARED_LIBS=OFF -DBUILD_OBJECT_LIBS=OFF
cmake --build build/ --target CoinToss