# Тестовое задание для Эрливидео
## Подготовка модели для tvm
```[bash]
./convert_from_onnx.sh
```
## Установка и запуск
```[bash]
mkdir build
cd build
cmake ..
make -j 6
./erlivideo-task
```