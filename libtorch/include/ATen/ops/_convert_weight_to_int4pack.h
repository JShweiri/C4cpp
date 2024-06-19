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



#include <ATen/ops/_convert_weight_to_int4pack_ops.h>

namespace at {


// aten::_convert_weight_to_int4pack(Tensor self, int innerKTiles) -> Tensor
inline at::Tensor _convert_weight_to_int4pack(const at::Tensor & self, int64_t innerKTiles) {
    return at::_ops::_convert_weight_to_int4pack::call(self, innerKTiles);
}

}
