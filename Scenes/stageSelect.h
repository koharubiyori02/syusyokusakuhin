//===========================================================
//
//　ステージセレクトシーン
// 
//===========================================================

#pragma once
#include "scene.h"
#include "..//polygon2D.h"

#define TEXTURE_NUM 2
  
class StageSelect : public Scene
{
private:
	Polygon2D* m_StageTexture[TEXTURE_NUM];//ステージ選択の画像
	bool m_MoveRight = false;//ステージ選択で右に動いているかどうか
	bool m_MoveLeft = false;//ステージ選択で左に動いているかどうか
	float m_FrameCount = 0.0f;//線形補間のカウント
	const float MOVE_FRAME = 6.0f;//何フレームで線形補間を完了させるか

	int m_SelectStage = 0;//どこを選択しているか
public:
	void Init() override;
	void Update() override;
	void Draw() override;
};