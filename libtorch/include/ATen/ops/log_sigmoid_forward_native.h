#pragma once

// @generated by torchgen/gen.py from NativeFunction.h

#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>
#include <c10/core/QScheme.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <tuple>
#include <vector>


namespace at {
namespace native {
TORCH_API ::std::tuple<at::Tensor,at::Tensor> log_sigmoid_forward_cpu(const at::Tensor & self);
TORCH_API ::std::tuple<at::Tensor &,at::Tensor &> log_sigmoid_forward_out_cpu(const at::Tensor & self, at::Tensor & output, at::Tensor & buffer);
TORCH_API ::std::tuple<at::Tensor,at::Tensor> log_sigmoid_forward_cuda(const at::Tensor & self);
TORCH_API ::std::tuple<at::Tensor &,at::Tensor &> log_sigmoid_forward_out_cuda(const at::Tensor & self, at::Tensor & output, at::Tensor & buffer);
TORCH_API ::std::tuple<at::Tensor,at::Tensor> log_sigmoid_forward_mps(const at::Tensor & self);
TORCH_API ::std::tuple<at::Tensor &,at::Tensor &> log_sigmoid_forward_out_mps(const at::Tensor & self, at::Tensor & output, at::Tensor & buffer);
} // namespace native
} // namespace at
