#!/bin/bash

sources=(
    spider_attack/base.hpp
    spider_attack/entity.hpp
    spider_attack/environment.hpp
    spider_attack/base.cpp
    spider_attack/entity.cpp
    spider_attack/environment.cpp
    main.cpp
)

for src in "${sources[@]}"; do
    sed -E 's/(#include ".+"|#pragma once)//g' "$src"
done
