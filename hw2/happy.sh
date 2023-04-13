#!/bin/bash

sources=(
    phah_titu/base.hpp
    phah_titu/entity.hpp
    phah_titu/environment.hpp
    phah_titu/base.cpp
    phah_titu/entity.cpp
    phah_titu/environment.cpp
    main2.cpp
)

for src in "${sources[@]}"; do
    sed -E 's/(#include ".+"|#pragma once)//g' "$src"
done
