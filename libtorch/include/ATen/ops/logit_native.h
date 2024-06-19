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
TORCH_API at::Tensor logit(const at::Tensor & self, c10::optional<double> eps=c10::nullopt);
TORCH_API at::Tensor & logit_out(const at::Tensor & self, c10::optional<double> eps, at::Tensor & out);
TORCH_API at::Tensor & logit_(at::Tensor & self, c10::optional<double> eps=c10::nullopt);
TORCH_API at::Tensor logit_mps(const at::Tensor & self, c10::optional<double> eps=c10::nullopt);
TORCH_API at::Tensor & logit_out_mps(const at::Tensor & self, c10::optional<double> eps, at::Tensor & out);
} // namespace native
} // namespace at
