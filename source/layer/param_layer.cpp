//
// Created by fss on 22-11-13.
//

#include <glog/logging.h>
#include "param_layer.hpp"

namespace kuiper_infer {
ParamLayer::ParamLayer(const std::string &layer_name,
                       const std::vector<std::shared_ptr<Blob>> &weights,
                       const std::vector<std::shared_ptr<Blob>> &bias)
    : Layer(layer_name), weights_(weights), bias_(bias) {

}

const std::vector<std::shared_ptr<Blob>> &ParamLayer::weights() const {
  return this->weights_;
}

const std::vector<std::shared_ptr<Blob>> &ParamLayer::bias() const {
  return this->bias_;
}

void ParamLayer::set_weights(const std::vector<std::shared_ptr<Blob>> &weights) {
  this->weights_ = weights;
}

void ParamLayer::set_bias(const std::vector<std::shared_ptr<Blob>> &bias) {
  this->bias_ = bias;
}

void ParamLayer::set_weights(const std::vector<double> &weights) {
  const uint32_t elem_size = weights.size();

  uint32_t weight_size = 0;
  const uint32_t batch_size = this->bias_.size();
  for (uint32_t i = 0; i < batch_size; ++i) {
    weight_size += this->bias_.at(i)->size();
  }
  CHECK_EQ(weight_size, elem_size);

  const uint32_t blob_size = elem_size / batch_size;
  for (uint32_t idx = 0; idx < batch_size; ++idx) {
    const uint32_t start_offset = idx * blob_size;
    const uint32_t end_offset = start_offset + blob_size;
    const auto &sub_values = std::vector<double>{weights.begin() + start_offset,
                                                 weights.begin() + end_offset};
    this->bias_.at(idx)->Fill(sub_values, false);
  }
}

void ParamLayer::set_bias(const std::vector<double> &bias) {
  const uint32_t elem_size = bias.size();

  uint32_t bias_size = 0;
  const uint32_t batch_size = this->bias_.size();
  for (uint32_t i = 0; i < batch_size; ++i) {
    bias_size += this->bias_.at(i)->size();
  }
  CHECK_EQ(bias_size, elem_size);

  const uint32_t blob_size = elem_size / batch_size;
  for (uint32_t idx = 0; idx < batch_size; ++idx) {
    const uint32_t start_offset = idx * blob_size;
    const uint32_t end_offset = start_offset + blob_size;
    const auto &sub_values = std::vector<double>{bias.begin() + start_offset,
                                                 bias.begin() + end_offset};
    this->bias_.at(idx)->Fill(sub_values, false);
  }
}

InferStatus ParamLayer::Forward(const std::vector<std::shared_ptr<Blob>> &inputs,
                                std::vector<std::shared_ptr<Blob>> &outputs) {
  return InferStatus::kInferUnknown;
}

}
