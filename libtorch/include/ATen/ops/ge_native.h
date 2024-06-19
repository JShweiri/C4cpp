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
#include <ATen/ops/ge_meta.h>

namespace at {
namespace native {
struct TORCH_API structured_ge_Scalar_out : public at::meta::structured_ge_Scalar {
void impl(const at::Tensor & self, const at::Scalar & other, const at::Tensor & out);
};
TORCH_API at::Tensor ge_scalar_nested(const at::Tensor & self, const at::Scalar & other);
struct TORCH_API structured_ge_scalar_out_mps : public at::meta::structured_ge_Scalar {
void impl(const at::Tensor & self, const at::Scalar & other, const at::Tensor & out);
};
TORCH_API at::Tensor ge_quantized_cpu(const at::Tensor & self, const at::Scalar & other);
TORCH_API at::Tensor & ge_out_quantized_cpu(const at::Tensor & self, const at::Scalar & other, at::Tensor & out);
struct TORCH_API structured_ge_Tensor_out : public at::meta::structured_ge_Tensor {
void impl(const at::Tensor & self, const at::Tensor & other, const at::Tensor & out);
};
struct TORCH_API structured_ge_tensor_out_mps : public at::meta::structured_ge_Tensor {
void impl(const at::Tensor & self, const at::Tensor & other, const at::Tensor & out);
};
TORCH_API at::Tensor ge_quantized_cpu(const at::Tensor & self, const at::Tensor & other);
TORCH_API at::Tensor & ge_out_quantized_cpu(const at::Tensor & self, const at::Tensor & other, at::Tensor & out);
} // namespace native
} // namespace at
