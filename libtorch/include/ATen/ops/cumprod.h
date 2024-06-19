#pragma once

// @generated by torchgen/gen.py from Function.h

#include <ATen/Context.h>
#include <ATen/DeviceGuard.h>
#include <ATen/TensorUtils.h>
#include <ATen/TracerMode.h>
#include <ATen/core/Generator.h>
#include <ATen/core/Reduction.h>
#include <ATen/core/Tensor.h>
#include <c10/core/Scalar.h>
#include <c10/core/Storage.h>
#include <c10/core/TensorOptions.h>
#include <c10/util/Deprecated.h>
#include <c10/util/Optional.h>



#include <ATen/ops/cumprod_ops.h>

namespace at {


// aten::cumprod(Tensor self, int dim, *, ScalarType? dtype=None) -> Tensor
inline at::Tensor cumprod(const at::Tensor & self, int64_t dim, c10::optional<at::ScalarType> dtype=c10::nullopt) {
    return at::_ops::cumprod::call(self, dim, dtype);
}

// aten::cumprod.out(Tensor self, int dim, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
inline at::Tensor & cumprod_out(at::Tensor & out, const at::Tensor & self, int64_t dim, c10::optional<at::ScalarType> dtype=c10::nullopt) {
    return at::_ops::cumprod_out::call(self, dim, dtype, out);
}
// aten::cumprod.out(Tensor self, int dim, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
inline at::Tensor & cumprod_outf(const at::Tensor & self, int64_t dim, c10::optional<at::ScalarType> dtype, at::Tensor & out) {
    return at::_ops::cumprod_out::call(self, dim, dtype, out);
}

// aten::cumprod.dimname(Tensor self, Dimname dim, *, ScalarType? dtype=None) -> Tensor
inline at::Tensor cumprod(const at::Tensor & self, at::Dimname dim, c10::optional<at::ScalarType> dtype=c10::nullopt) {
    return at::_ops::cumprod_dimname::call(self, dim, dtype);
}

// aten::cumprod.dimname_out(Tensor self, Dimname dim, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
inline at::Tensor & cumprod_out(at::Tensor & out, const at::Tensor & self, at::Dimname dim, c10::optional<at::ScalarType> dtype=c10::nullopt) {
    return at::_ops::cumprod_dimname_out::call(self, dim, dtype, out);
}
// aten::cumprod.dimname_out(Tensor self, Dimname dim, *, ScalarType? dtype=None, Tensor(a!) out) -> Tensor(a!)
inline at::Tensor & cumprod_outf(const at::Tensor & self, at::Dimname dim, c10::optional<at::ScalarType> dtype, at::Tensor & out) {
    return at::_ops::cumprod_dimname_out::call(self, dim, dtype, out);
}

}
