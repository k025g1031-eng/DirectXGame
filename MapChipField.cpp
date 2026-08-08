#include "MapChipField.h"

#include <cassert>
#include <fstream>
#include <map>
#include <sstream>
#include <string>

namespace {

std::map<std::string, MapChipType> mapChipTable = {
    {"0", MapChipType::kBlank},
    {"1", MapChipType::kBlock},
};

}

MapChipField::MapChipField() {}

MapChipField::~MapChipField() {}

void MapChipField::Initialize() {

	// マップチップデータのサイズを設定
	mapChipData_.data.resize(kNumBlockVirtical);

	for (uint32_t i = 0; i < kNumBlockVirtical; ++i) {

		mapChipData_.data[i].resize(kNumBlockHorizontal, MapChipType::kBlank);
	}
}

void MapChipField::LoadMapChipCsv(const std::string& filePath) {

	std::ifstream file(filePath);

	assert(file.is_open());

	std::string line;

	uint32_t y = 0;

	while (std::getline(file, line)) {

		std::stringstream lineStream(line);

		std::string cell;

		uint32_t x = 0;

		while (std::getline(lineStream, cell, ',')) {

			if (y >= kNumBlockVirtical) {
				break;
			}

			if (x >= kNumBlockHorizontal) {
				break;
			}

			auto it = mapChipTable.find(cell);

			if (it != mapChipTable.end()) {
				mapChipData_.data[y][x] = it->second;
			}

			++x;
		}

		++y;
	}
}

MapChipType MapChipField::GetMapChipTypeByIndex(uint32_t x, uint32_t y) const {

	if (x >= kNumBlockHorizontal || y >= kNumBlockVirtical) {

		return MapChipType::kBlank;
	}

	return mapChipData_.data[y][x];
}

KamataEngine::Vector3 MapChipField::GetMapChipPositionByIndex(uint32_t x, uint32_t y) const {

	KamataEngine::Vector3 position{};

	position.x = kBlockWidth * x;
	position.y = kBlockHeight * y;
	position.z = 0.0f;

	return position;
}