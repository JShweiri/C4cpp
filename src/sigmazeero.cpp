#include "sigmazero.hpp"
#include <torch/serialize.h>

SigmaZeroNet::SigmaZeroNet() {
    // Define the layers of the neural network
    conv1 = register_module("conv1", torch::nn::Conv2d(torch::nn::Conv2dOptions(3, 64, /*kernel_size=*/3).stride(1).padding(1)));
    conv2 = register_module("conv2", torch::nn::Conv2d(torch::nn::Conv2dOptions(64, 128, /*kernel_size=*/3).stride(1).padding(1)));
    conv3 = register_module("conv3", torch::nn::Conv2d(torch::nn::Conv2dOptions(128, 256, /*kernel_size=*/3).stride(1).padding(1)));

    fc1 = register_module("fc1", torch::nn::Linear(256 * 6 * 7, 1024));
    fc_value = register_module("fc_value", torch::nn::Linear(1024, 1));
    fc_policy = register_module("fc_policy", torch::nn::Linear(1024, 7)); // 7 possible moves in Connect 4

    value_head = register_module("value_head", torch::nn::Linear(1024, 1));
    policy_head = register_module("policy_head", torch::nn::Linear(1024, 7));
}

std::pair<torch::Tensor, torch::Tensor> SigmaZeroNet::forward(torch::Tensor x) {
    x = torch::relu(conv1->forward(x));
    x = torch::relu(conv2->forward(x));
    x = torch::relu(conv3->forward(x));

    x = x.view({-1, 256 * 6 * 7}); // Flatten the tensor

    x = torch::relu(fc1->forward(x));

    torch::Tensor value = torch::tanh(fc_value->forward(x));
    torch::Tensor policy = torch::softmax(fc_policy->forward(x), /*dim=*/1);

    return std::make_pair(value, policy);
}

void SigmaZeroNet::save_weights(const std::string& file_path) {
    torch::serialize::OutputArchive archive;
    this->save(archive);
    archive.save_to(file_path);
}

void SigmaZeroNet::load_weights(const std::string& file_path) {
    torch::serialize::InputArchive archive;
    archive.load_from(file_path);
    this->load(archive);
}
