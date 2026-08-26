/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <cstdint>
#include <string>
#include <functional>
#include "../bmi270/src/bmi270.h"

/* -------------------------------------------------------------------------- */
/*                                    Misc                                    */
/* -------------------------------------------------------------------------- */
#define APP_VERSION "V0.2"

/* -------------------------------------------------------------------------- */
/*                                Service mode                                */
/* -------------------------------------------------------------------------- */
namespace ServiceMode {
enum ServiceMode_t {
    mode_none = 0,
    mode_uvc,
    mode_web_server,
};
};

/* -------------------------------------------------------------------------- */
/*                                     IMU                                    */
/* -------------------------------------------------------------------------- */
namespace IMU {
struct ImuData_t {
    float accelX = 0.0f;
    float accelY = 0.0f;
    float accelZ = 0.0f;

    float gyroX = 0.0f;
    float gyroY = 0.0f;
    float gyroZ = 0.0f;

    float magX = -1.0f;
    float magY = -1.0f;
    float magZ = -1.0f;
};
}  // namespace IMU

namespace SHARED_DATA {
struct SharedData_t {
    // Add your shared data here
    // ...

    ServiceMode::ServiceMode_t service_mode = ServiceMode::mode_none;

    // IMU
    BMI270_Class* imu = nullptr;
    bool is_bmm150_ok = false;
    IMU::ImuData_t imu_data;
};
}  // namespace SHARED_DATA
