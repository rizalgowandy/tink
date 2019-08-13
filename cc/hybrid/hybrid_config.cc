// Copyright 2018 Google Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
///////////////////////////////////////////////////////////////////////////////

#include "tink/hybrid/hybrid_config.h"

#include "absl/memory/memory.h"
#include "tink/aead/aead_config.h"
#include "tink/hybrid/ecies_aead_hkdf_private_key_manager.h"
#include "tink/hybrid/ecies_aead_hkdf_public_key_manager.h"
#include "tink/config/config_util.h"
#include "tink/registry.h"
#include "tink/hybrid/hybrid_decrypt_wrapper.h"
#include "tink/hybrid/hybrid_encrypt_wrapper.h"
#include "tink/util/status.h"
#include "proto/config.pb.h"

using google::crypto::tink::RegistryConfig;

namespace crypto {
namespace tink {

constexpr char HybridConfig::kHybridDecryptCatalogueName[];
constexpr char HybridConfig::kHybridDecryptPrimitiveName[];
constexpr char HybridConfig::kHybridEncryptCatalogueName[];
constexpr char HybridConfig::kHybridEncryptPrimitiveName[];

// static
const RegistryConfig& HybridConfig::Latest() {
  static const RegistryConfig* config = new RegistryConfig();
  return *config;
}

// static
util::Status HybridConfig::Register() {
  auto status = AeadConfig::Register();

  // Register key managers.
  if (!status.ok()) return status;
  status = Registry::RegisterKeyManager(
      absl::make_unique<EciesAeadHkdfPrivateKeyManager>(), true);
  if (!status.ok()) return status;
  status = Registry::RegisterKeyManager(
      absl::make_unique<EciesAeadHkdfPublicKeyManager>(), true);
  if (!status.ok()) return status;

  // Register primitive wrappers.
  status = Registry::RegisterPrimitiveWrapper(
      absl::make_unique<HybridEncryptWrapper>());
  if (!status.ok()) return status;
  return Registry::RegisterPrimitiveWrapper(
      absl::make_unique<HybridDecryptWrapper>());
}

}  // namespace tink
}  // namespace crypto
