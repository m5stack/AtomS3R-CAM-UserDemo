/*
 * SPDX-FileCopyrightText: 2024 M5Stack Technology CO LTD
 *
 * SPDX-License-Identifier: MIT
 */
#include <stdio.h>
#include <assets.h>

int main(int, char**)
{
    // Create and output to bin
    StaticAsset_t* asset_pool = AssetPool::CreateStaticAsset();
    if (asset_pool == nullptr) return 1;

    const bool result = AssetPool::CreateStaticAssetBin(asset_pool);
    delete asset_pool;
    return result ? 0 : 1;
}
