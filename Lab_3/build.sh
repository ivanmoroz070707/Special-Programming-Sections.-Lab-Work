#!/bin/bash

# Стоп при любой ошибке
set -e

# Создаем папку build если её нет
mkdir -p build
cd build

# Генерация проекта
cmake ..

# Сборка
make -j$(nproc)

echo "Build finished successfully!"

