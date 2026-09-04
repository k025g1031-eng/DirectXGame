#pragma once
#include "KamataEngine.h"
#include <cstdint>
#include <string>
#include <vector>

/// <summary>
/// マップチップの種別
/// </summary>
enum class MapChipType {
	kBlank, // 空白
	kBlock, // ブロック
};

/// <summary>
/// マップチップデータ
/// </summary>
struct MapChipData {
	std::vector<std::vector<MapChipType>> data;
};

/// <summary>
/// マップチップのインデックス
/// </summary>
struct MapChipIndexSet {
	uint32_t xIndex;
	uint32_t yIndex;
};

/// <summary>
/// マップチップの境界
/// </summary>
struct MapChipRect {
	float left;
	float right;
	float bottom;
	float top;
};

/// <summary>
/// マップチップフィールド
/// </summary>
class MapChipField {
public:
	// 1ブロックのサイズ
	static inline const float kBlockWidth = 1.0f;
	static inline const float kBlockHeight = 1.0f;

	// ブロックの個数
	static inline const uint32_t kNumBlockVirtical = 20;
	static inline const uint32_t kNumBlockHorizontal = 100;

public:
	MapChipField();
	~MapChipField();

	/// <summary>
	/// マップチップデータの初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// CSVファイルからマップチップデータを読み込む
	/// </summary>
	/// <param name="filePath">CSVファイルのパス</param>
	void LoadMapChipCsv(const std::string& filePath);

	/// <summary>
	/// マップチップの種類を取得する
	/// </summary>
	/// <param name="x">横方向の番号</param>
	/// <param name="y">縦方向の番号</param>
	/// <returns>マップチップの種類</returns>
	MapChipType GetMapChipTypeByIndex(uint32_t x, uint32_t y) const;

	/// <summary>
	/// マップチップのワールド座標を取得する
	/// </summary>
	/// <param name="x">横方向の番号</param>
	/// <param name="y">縦方向の番号</param>
	/// <returns>ワールド座標</returns>
	KamataEngine::Vector3 GetMapChipPositionByIndex(uint32_t x, uint32_t y) const;

	MapChipIndexSet GetMapChipIndexSetByPosition(const KamataEngine::Vector3& position) const;

	MapChipRect GetRectByIndex(uint32_t x, uint32_t y) const;

	/// <summary>
	/// マップチップデータを取得する
	/// </summary>
	/// <returns>マップチップデータ</returns>
	const MapChipData& GetMapChipData() const { return mapChipData_; }

private:
	// マップチップデータ
	MapChipData mapChipData_;
};
