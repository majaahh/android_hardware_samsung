//
// Copyright 2016 The Android Open Source Project
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//

#pragma once

#include <functional>
#include <vector>

#include "async_fd_watcher.h"
#include "bt_vendor_lib.h"
#include "h4_protocol.h"

namespace aidl::android::hardware::bluetooth::impl {

using InitializeCompleteCallback = std::function<void(bool success)>;
using PacketReadCallback = std::function<void(const std::vector<uint8_t>&)>;

class FirmwareStartupTimer;

class VendorInterface {
  public:
    static bool Initialize(InitializeCompleteCallback initialize_complete_cb,
                           PacketReadCallback event_cb, PacketReadCallback acl_cb,
                           PacketReadCallback sco_cb, PacketReadCallback iso_cb);
    static void Shutdown();
    static VendorInterface* get();

    size_t Send(uint8_t type, const uint8_t* data, size_t length);

    void OnFirmwareConfigured(uint8_t result);

  private:
    virtual ~VendorInterface() = default;

    bool Open(InitializeCompleteCallback initialize_complete_cb, PacketReadCallback event_cb,
              PacketReadCallback acl_cb, PacketReadCallback sco_cb, PacketReadCallback iso_cb);
    void Close();

    void OnTimeout();

    void HandleIncomingEvent(const std::vector<uint8_t>& hci_packet);

    void* lib_handle_ = nullptr;
    bt_vendor_interface_t* lib_interface_ = nullptr;
    ::android::hardware::bluetooth::async::AsyncFdWatcher fd_watcher_;
    InitializeCompleteCallback initialize_complete_cb_;
    ::android::hardware::bluetooth::hci::H4Protocol* hci_ = nullptr;

    PacketReadCallback event_cb_;

    FirmwareStartupTimer* firmware_startup_timer_ = nullptr;
};

}  // namespace aidl::android::hardware::bluetooth::impl
