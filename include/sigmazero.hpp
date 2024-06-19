#ifndef SIGMA_ZERO_NET_HPP
#define SIGMA_ZERO_NET_HPP

#include <torch/torch.h>
#include "board.hpp"

class SigmaZeroNet : public torch::nn::Module {
public:
    SigmaZeroNet();

    std::pair<torch::Tensor, torch::Tensor> forward(torch::Tensor x);

    void save_weights(const std::string& file_path);
    void load_weights(const std::string& file_path);

private:
    torch::nn::Conv2d conv1{nullptr}, conv2{nullptr}, conv3{nullptr};
    torch::nn::Linear fc1{nullptr}, fc_value{nullptr}, fc_policy{nullptr};
    torch::nn::Linear value_head{nullptr}, policy_head{nullptr};
};

#endif // SIGMA_ZERO_NET_HPP
