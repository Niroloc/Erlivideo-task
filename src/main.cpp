#include <tvm/runtime/module.h>
#include <opencv2/opencv.hpp>


int main(int argc, char* argv[])
{
    if (argc == 1) {
        std::string libmodel = argv[0];
        std::cout << "Start" << std::endl;
        auto model = tvm::runtime::Module::LoadFromFile(libmodel);
        std::cout << "Loaded" << std::endl;
    }
    return 0;
}