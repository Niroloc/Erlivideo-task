#include <fstream>
#include <iostream>
#include <filesystem>
#include <tvm/runtime/module.h>
#include <tvm/runtime/registry.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/opencv.hpp>


int main(int argc, char* argv[])
{
    if (argc == 3) {
        DLDevice dev{kDLCPU, 0};
        // load lib module
        std::filesystem::path libmodelfold {argv[1]};
        std::filesystem::path inf_object {argv[2]};
        std::cout << "Start" << std::endl;
        auto loaded_lib = tvm::runtime::Module::LoadFromFile(std::string(libmodelfold/"mod.so"));
        // load json
        std::ifstream loaded_json(libmodelfold/"mod.json");
        std::string json_data((std::istreambuf_iterator<char>(loaded_json)), std::istreambuf_iterator<char>());
        loaded_json.close();
        // load params
        std::ifstream loaded_params(libmodelfold/"deploy_param.params", std::ios::binary);
        std::string params_data((std::istreambuf_iterator<char>(loaded_params)), std::istreambuf_iterator<char>());
        loaded_params.close();
        TVMByteArray params_arr;
        params_arr.data = params_data.c_str();
        params_arr.size = params_data.length();
        int device_type = dev.device_type;
        tvm::runtime::Module mod = (*tvm::runtime::Registry::Get("tvm.graph_executor.create"))(json_data, loaded_lib, device_type, dev.device_id);

        tvm::runtime::PackedFunc load_params = mod.GetFunction("load_params");
        load_params(params_arr);

        tvm::runtime::PackedFunc set_input = mod.GetFunction("set_input");
        tvm::runtime::PackedFunc get_output = mod.GetFunction("get_output");
        tvm::runtime::PackedFunc run = mod.GetFunction("run");

        std::cout << "Module loaded successfully" << std::endl;


        auto f32 = tvm::runtime::DataType::Float(32);
        tvm::runtime::NDArray x = tvm::runtime::NDArray::Empty({3, 416, 416}, f32, dev);
        tvm::runtime::NDArray y = tvm::runtime::NDArray::Empty({1, 80}, f32, dev);

        cv::Mat img = cv::imread(inf_object);
        std::cout << "Picture loaded" << std::endl;

        cv::Mat resized;
        cv::resize(img, resized, cv::Size(416, 416));
        for (int i = 0; i < 3 * 416 * 416; ++i){
            static_cast<float*>(x->data)[i] = resized.data[i];
        }

        // run model
        set_input("x", x);
        run();
        get_output("y", y);
    }
    else {
        std::cout << "Start program with command erlivideo-task /path/to/mod.so/file /path/to/pic/or/video" << std::endl;
    }
    return 0;
}