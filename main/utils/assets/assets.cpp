/*
 * SPDX-FileCopyrightText: 2025 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include "assets.h"
#include <mooncake.h>
#include <algorithm>
#include <cstddef>
#include <cstring>
#include <iterator>
#include <string>
#ifndef ESP_PLATFORM
#include "images/images.h"
#include <iostream>
#include <fstream>
#endif

AssetPool* AssetPool::_asset_pool = nullptr;

AssetPool* AssetPool::Get()
{
    if (_asset_pool == nullptr) _asset_pool = new AssetPool;
    return _asset_pool;
}

StaticAsset_t* AssetPool::getStaticAsset()
{
    if (_data.static_asset == nullptr) {
        spdlog::error("static asset not exsit");
        return nullptr;
    }
    return _data.static_asset;
}

bool AssetPool::injectStaticAsset(StaticAsset_t* asset)
{
    if (_data.static_asset != nullptr) {
        spdlog::error("static asset already exist");
        return false;
    }

    if (asset == nullptr) {
        spdlog::error("invalid static asset ptr");
        return false;
    }

    _data.static_asset = asset;

    // // Default local text map
    // setLocalTextTo(_data.locale_code);

    spdlog::info("static asset injected");
    return true;
}

/* -------------------------------------------------------------------------- */
/*                            Static asset generate                           */
/* -------------------------------------------------------------------------- */
#ifdef PLATFORM_BUILD_DESKTOP

/**
 * @brief Copy file into target as binary
 *
 * @param filePath
 * @param target
 * @param targetSize
 * @return true
 * @return false
 */
static bool _copy_file(const std::string& filePath, uint8_t* target, std::size_t targetSize)
{
    spdlog::info("try open {}", filePath);

    std::ifstream file(filePath, std::ios::binary | std::ios::ate);
    if (!file.is_open()) {
        spdlog::error("open failed: {}", filePath);
        return false;
    }

    const std::streamoff file_size = file.tellg();
    if (file_size < 0) {
        spdlog::error("get file size failed: {}", filePath);
        return false;
    }

    if (static_cast<std::size_t>(file_size) != targetSize) {
        spdlog::error("asset size mismatch: {} is {} bytes, target array is {} bytes", filePath, file_size, targetSize);
        return false;
    }

    file.seekg(0, std::ios::beg);
    spdlog::info("file binary size {}", file_size);

    if (target == nullptr) {
        spdlog::error("invalid target buffer for: {}", filePath);
        return false;
    }

    file.read(reinterpret_cast<char*>(target), static_cast<std::streamsize>(targetSize));
    if (!file) {
        spdlog::error("read failed: {}", filePath);
        return false;
    }

    return true;
}

/**
 * @brief Convert png image into rgb565 and copy into target as binary
 *
 * @param filePath
 * @param target
 * @return true
 * @return false
 */
#ifdef ENABLE_PNG_CONVERTOR
static bool _copy_png_image(std::string filePath, uint16_t* target)
{
    spdlog::info("try convert: {}", filePath);
    size_t output_length = 0;
    int width = 0, height = 0;

    ImageConversionError result = convertPNGToR5G6B5(filePath.c_str(), target, &output_length, &width, &height);

    if (result != ImageConversionError::Success) {
        spdlog::error("convert failed: {}", static_cast<int>(result));
        return false;
    }

    spdlog::info("ok, image size: {} x {}, array length: {}", width, height, output_length);
    return true;
}
#endif

StaticAsset_t* AssetPool::CreateStaticAsset()
{
    auto asset_pool = new StaticAsset_t{};

    const bool html_ok = _copy_file("../../main/utils/assets/images/index.html.gz", asset_pool->Image.index_html_gz,
                                    sizeof(asset_pool->Image.index_html_gz));
    const bool logo_ok = _copy_file("../../main/utils/assets/images/m5.jpg", asset_pool->Image.m5_logo,
                                    sizeof(asset_pool->Image.m5_logo));

    if (!html_ok || !logo_ok) {
        delete asset_pool;
        return nullptr;
    }

    return asset_pool;
}

bool AssetPool::CreateStaticAssetBin(StaticAsset_t* assetPool)
{
    if (assetPool == nullptr) {
        spdlog::error("invalid static asset");
        return false;
    }

    /* -------------------------------------------------------------------------- */
    /*                                Output to bin                               */
    /* -------------------------------------------------------------------------- */
    std::string bin_path = "../output/AssetPool.bin";

    std::ofstream outFile(bin_path, std::ios::binary);
    if (!outFile) {
        spdlog::error("open {} failed", bin_path);
        return false;
    }

    outFile.write(reinterpret_cast<const char*>(assetPool), sizeof(StaticAsset_t));
    if (!outFile) {
        spdlog::error("write {} failed", bin_path);
        return false;
    }

    outFile.close();
    spdlog::info("output asset pool to: {}", bin_path);
    return true;
}

StaticAsset_t* AssetPool::GetStaticAssetFromBin()
{
    auto asset_pool = new StaticAsset_t;

    // Read from bin
    std::string bin_path = "../output/AssetPool.bin";

    std::ifstream inFile(bin_path, std::ios::binary);
    if (!inFile) spdlog::error("open {} failed", bin_path);

    inFile.read(reinterpret_cast<char*>(asset_pool), sizeof(StaticAsset_t));
    inFile.close();

    // // Test
    // for (int i = 0; i < 10; i++)
    // {
    //     spdlog::info(
    //         "0x{:X} 0x{:X}", asset_pool->Font.montserrat_semibold_14[i],
    //         asset_pool->Font.montserrat_semibolditalic_72[i]);
    // }

    spdlog::info("load asset pool from: {}", bin_path);
    return asset_pool;
}
#else

StaticAsset_t* AssetPool::CreateStaticAsset()
{
    auto asset_pool = new StaticAsset_t;

    // ...

    return asset_pool;
}

#endif
