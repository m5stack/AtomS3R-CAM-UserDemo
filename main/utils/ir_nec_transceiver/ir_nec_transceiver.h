/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#pragma once
#include <stdint.h>

void ir_nec_transceiver_init(int txPin);
void ir_nec_transceiver_send(uint16_t addr, uint16_t cmmd);
