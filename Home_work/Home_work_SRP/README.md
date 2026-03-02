Інструкція до роботи програми з камерою
В bash:
### Оновлюємо систему
sudo apt update
sudo apt upgrade -y
### Встановлення необхідних пакетів
sudo add-apt-repository universe
sudo apt update
sudo apt install libopencv-dev build-essential pkg-config cmake git -y
### Перевірка встановлення OpenCV
pkg-config --modversion opencv4
### Компіляція за допомогою g++
g++ main.cpp -o webcam `pkg-config --cflags --libs opencv4`
### Запуск програми
./webcam