#include "DxLib.h"

//画面の縦幅
#define ScreenSizeX 600
//画面の横幅
#define ScreenSizeY 600

//タイトル背景
int title_bg;
//シーン移動の際に使用するボタン
int button;
//時間計測
int startTime;
//パネルの縦幅と横幅の大きさ
int panel_length;
int panel_width;
//フォント
int text_font01;
int text_font02;

//パネル
int stage1_pics[9];
int stage2_pics[16];
int stage3_pics[25];
int *stage_panel;
int stage1_panel;
int stage2_panel;
int stage3_panel;
int complete_panel;
int *panel;

//ゲームの状態
enum { Title, Game, Clear } status;
bool isGame;
enum { NotChoice ,Stage1, Stage2, Stage3} stage;

void DrawTextAndGraph() {
	DrawGraph(0, 0, title_bg, FALSE);
	DrawGraph(200, 200, button, FALSE);
	DrawStringToHandle(240, 220, "ステージ1", GetColor(0, 0, 0), text_font02);
	DrawGraph(200, 300, button, FALSE);
	DrawStringToHandle(240, 320, "ステージ2", GetColor(0, 0, 0), text_font02);
	DrawGraph(200, 400, button, FALSE);
	DrawStringToHandle(240, 420, "ステージ3", GetColor(0, 0, 0), text_font02);
	DrawStringToHandle(125, 60, "そろえてパズル!", GetColor(255, 0, 0), text_font01);


	/*デバッグ用
	//マウスのx,y座標
	int x = 0;
	int y = 0;
	//マウス座標を確認
	GetMousePoint(&x, &y);
	SetFontSize(24);
	DrawFormatString(20, 50, GetColor(0, 0, 255), "x座標は%d", x);
	DrawFormatString(20, 100, GetColor(0, 0, 255), "y座標は%d", y);
	*/
}

void CheckTapPoint(int x, int y) {
	//左クリック中
	if (GetMouseInput() && MOUSE_INPUT_LEFT != 0) {
		GetMousePoint(&x, &y);
		if (x > 210 && x < 390) {
			//ボタン1押下
			if (y > 200 && y < 260) {
				complete_panel = stage1_panel;
				stage_panel = stage1_pics;
				panel_length = panel_width = 3;
				//ステージ1スタート
				stage = Stage1;
			}
			//ボタン2押下
			else if (y > 300 && y < 360) {
				complete_panel = stage2_panel;
				stage_panel = stage2_pics;
				panel_length = panel_width = 4;
				//ステージ2スタート
				stage = Stage2;
			}
			//ボタン3押下
			else if (y > 400 && y < 460) {
				complete_panel = stage3_panel;
				stage_panel = stage3_pics;
				panel_length = panel_width = 5;
				//ステージ3スタート
				stage = Stage3;
			}
		}
	}
}

void GameTitle() {
	//画像とテキストの表示
	DrawTextAndGraph();

	//x,y座標
	int x = 0;
	int y = 0;
	//タッチ座標を確認
	CheckTapPoint(x, y);

	if (stage != NotChoice && GetNowCount() - startTime > 1000) {
		PlaySoundFile("button70.mp3", DX_PLAYTYPE_BACK);
		status = Game;
		startTime = GetNowCount();
	}

}

void PanelShaffle(int* length) {
	int *changePanel = new int[panel_length*panel_width];
	for (int i = 0; i < *length; i++) {
		panel[i] = i;
		changePanel[i] = i;
	}
	
	//ピースをシャッフル
	for (int i = 0; i < *length; i++) {
		int changeNum = GetRand(*length - i);
		panel[i] = panel[changeNum];
		panel[changeNum] = changePanel[i];
		changePanel[i] = panel[i];
		changePanel[changeNum] = panel[changeNum];
	}
}

void GameStart(int* length) {
	//パネルの大きさ決定
	panel = new int[panel_length*panel_width];
	//お題の画像表示
	DrawGraph(0, 0, complete_panel, FALSE);
	//3秒停止
	while (GetNowCount() - startTime < 3000) {
		// メッセージ
		DrawStringToHandle (90, 80, "この絵をそろえてね", GetColor(0, 10, 0), text_font01);
		ScreenFlip();
	}

	PanelShaffle(length);
	isGame = true;
	status = Game;
}

void ChangePiece(int x, int y, int* length) {
	int p1 = y * panel_length + x;
	int p2 = -1;
	if (x > 0 && panel[p1 - 1] == *length - 1) p2 = p1 - 1;
	if (x < panel_width - 1 && panel[p1 + 1] == *length - 1) p2 = p1 + 1;
	if (y > 0 && panel[p1 - panel_length] == *length - 1) p2 = p1 - panel_length;
	if (y < panel_length - 1 && panel[p1 + panel_length] == *length - 1) p2 = p1 + panel_length;
	if (p2 != -1) {
		panel[p2] = panel[p1];
		panel[p1] = *length - 1;
		//ピースを動かしたときの効果音再生
		PlaySoundFile("button68.mp3", DX_PLAYTYPE_BACK);
	}
}

void GameMain() {
	int length = panel_length * panel_width;

	//ゲーム中
	if (isGame) {
		bool clear = true;

		if (GetMouseInput() && MOUSE_INPUT_LEFT != 0) {
			int x;
			int y;
			GetMousePoint(&x, &y);
			ChangePiece(x / (ScreenSizeX / panel_width), y / (ScreenSizeY / panel_length), &length);
		}


		for (int i = 0; i < length; i++) {
			//パズルがそろってなかったら
			if (panel[i] != i) {
				clear = false;
			}
			//パネルの表示
			if (panel[i] < length - 1) {
				DrawGraph((i % panel_width) * (ScreenSizeX / panel_width), (i / panel_length) * (ScreenSizeY / panel_length), stage_panel[panel[i]], FALSE);
			}
		}
		//クリアシーンへ
		if (clear) {
			startTime = GetNowCount();
			status = Clear;
		}
	}
	else 
	{
		GameStart(&length);
	}
}

// 終了画面
void GameClear() {
	DrawGraph(0, 0, complete_panel, FALSE);
	DrawStringToHandle(130, 110, "よくできました!", GetColor(0, 10, 0), text_font01);

	//画面タッチでタイトル画面へ
	if (GetMouseInput() && MOUSE_INPUT_LEFT != 0 && GetNowCount() - startTime > 2000) {
		stage = NotChoice;
		isGame = false;
		status = Title;
		startTime = GetNowCount();
	}
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	if (DxLib_Init() == -1) {
		return -1;
	}

	SetGraphMode(ScreenSizeX, ScreenSizeY, 32);
	ChangeWindowMode(TRUE);
	SetDrawScreen(DX_SCREEN_BACK);

	//画像の読みこみ
	LoadDivGraph("Sprites/pic1.png", 9, 3, 3, 200, 200, stage1_pics, FALSE);
	LoadDivGraph("Sprites/pic2.png", 16, 4, 4, 150, 150, stage2_pics, FALSE);
	LoadDivGraph("Sprites/pic3.jpg", 25, 5, 5, 120, 120, stage3_pics, FALSE);
	//画像の表示
	stage1_panel = LoadGraph("Sprites/pic1.png");
	stage2_panel = LoadGraph("Sprites/pic2.png");
	stage3_panel = LoadGraph("Sprites/pic3.jpg");
	title_bg = LoadGraph("Sprites/title.jpg");
	button = LoadGraph("Sprites/button.png");
	//フォントの作成
	text_font01 = CreateFontToHandle("メイリオ", 48, 5, DX_FONTTYPE_ANTIALIASING);
	text_font02 = CreateFontToHandle("MS明朝", 24, 3, DX_FONTTYPE_ANTIALIASING);

	//初期化
	startTime = 0;
	status = Title;
	//BGM再生
	PlayMusic("c2.mp3", DX_PLAYTYPE_LOOP);

	while (!ProcessMessage()) {
		ClearDrawScreen();
		//シーンの状態別に更新
		switch (status) {
		case Title:
			GameTitle();
			break;
		case Game:
			GameMain();
			break;
		case Clear:
			GameClear();
			break;
		default:
			status = Title;
			break;
		}

		ScreenFlip();
	}

	DxLib_End();

	return 0;
}
