#!/bin/bash

tvmc compile --target "llvm" --output yolo4.tar yolo4-416x416f32.onnx

tar -xvf yolo4.tar

mkdir model
mv mod.* model/

rm yolo4.tar
