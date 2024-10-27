/*
 * Copyright (C) 2022 The Android Open Source Project
 *
 * SPDX-License-Identifier: Apache-2.0
 */
#include <android-base/logging.h>
#include <android/binder_manager.h>
#include <android/binder_process.h>

#include "Thermal.h"

constexpr std::string_view kThermalLogTag("pixel-thermal");

using ::android::OK;
using ::android::status_t;

// Generated AIDL files:
using Thermal = ::aidl::android::hardware::thermal::implementation::Thermal;

#if !defined(THERMAL_INSTANCE_NAME)
#define THERMAL_INSTANCE_NAME "default"
#endif

int main(int /* argc */, char ** /* argv */) {
    android::base::SetDefaultTag(kThermalLogTag.data());

    auto svc = ndk::SharedRefBase::make<Thermal>();
    const auto svcName = std::string() + svc->descriptor + "/" + THERMAL_INSTANCE_NAME;
    LOG(INFO) << "Pixel Thermal AIDL Service starting..." + svcName;
    ABinderProcess_setThreadPoolMaxThreadCount(0);

    auto svcBinder = svc->asBinder();
    binder_status_t status = AServiceManager_addService(svcBinder.get(), svcName.c_str());
    if (status != STATUS_OK) {
        LOG(ERROR) << "Pixel Thermal AIDL Service failed to start: " << status << ".";
        return EXIT_FAILURE;
    }
    LOG(INFO) << "Pixel Thermal HAL AIDL Service started.";
    ABinderProcess_joinThreadPool();
    return EXIT_FAILURE;  // should not reach
}
