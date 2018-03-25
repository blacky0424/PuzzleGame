#include "DxLib.h"

//��ʂ̏c��
#define ScreenSizeX 600
//��ʂ̉���
#define ScreenSizeY 600

//�^�C�g���w�i
int title_bg;
//�V�[���ړ��̍ۂɎg�p����{�^��
int button;
//���Ԍv��
int startTime;
//�p�l���̏c���Ɖ����̑傫��
int panel_length;
int panel_width;
//�t�H���g
int text_font01;
int text_font02;

//�p�l��
int stage1_pics[9];
int stage2_pics[16];
int stage3_pics[25];
int *stage_panel;
int stage1_panel;
int stage2_panel;
int stage3_panel;
int complete_panel;
int *panel;

//�Q�[���̏��
enum { Title, Game, Clear } status;
bool isGame;
enum { NotChoice ,Stage1, Stage2, Stage3} stage;

void DrawTextAndGraph() {
	DrawGraph(0, 0, title_bg, FALSE);
	DrawGraph(200, 200, button, FALSE);
	DrawStringToHandle(240, 220, "�X�e�[�W1", GetColor(0, 0, 0), text_font02);
	DrawGraph(200, 300, button, FALSE);
	DrawStringToHandle(240, 320, "�X�e�[�W2", GetColor(0, 0, 0), text_font02);
	DrawGraph(200, 400, button, FALSE);
	DrawStringToHandle(240, 420, "�X�e�[�W3", GetColor(0, 0, 0), text_font02);
	DrawStringToHandle(125, 60, "���낦�ăp�Y��!", GetColor(255, 0, 0), text_font01);


	/*�f�o�b�O�p
	//�}�E�X��x,y���W
	int x = 0;
	int y = 0;
	//�}�E�X���W���m�F
	GetMousePoint(&x, &y);
	SetFontSize(24);
	DrawFormatString(20, 50, GetColor(0, 0, 255), "x���W��%d", x);
	DrawFormatString(20, 100, GetColor(0, 0, 255), "y���W��%d", y);
	*/
}

void CheckTapPoint(int x, int y) {
	//���N���b�N��
	if (GetMouseInput() && MOUSE_INPUT_LEFT != 0) {
		GetMousePoint(&x, &y);
		if (x > 210 && x < 390) {
			//�{�^��1����
			if (y > 200 && y < 260) {
				complete_panel = stage1_panel;
				stage_panel = stage1_pics;
				panel_length = panel_width = 3;
				//�X�e�[�W1�X�^�[�g
				stage = Stage1;
			}
			//�{�^��2����
			else if (y > 300 && y < 360) {
				complete_panel = stage2_panel;
				stage_panel = stage2_pics;
				panel_length = panel_width = 4;
				//�X�e�[�W2�X�^�[�g
				stage = Stage2;
			}
			//�{�^��3����
			else if (y > 400 && y < 460) {
				complete_panel = stage3_panel;
				stage_panel = stage3_pics;
				panel_length = panel_width = 5;
				//�X�e�[�W3�X�^�[�g
				stage = Stage3;
			}
		}
	}
}

void GameTitle() {
	//�摜�ƃe�L�X�g�̕\��
	DrawTextAndGraph();

	//x,y���W
	int x = 0;
	int y = 0;
	//�^�b�`���W���m�F
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
	
	//�s�[�X���V���b�t��
	for (int i = 0; i < *length; i++) {
		int changeNum = GetRand(*length - i);
		panel[i] = panel[changeNum];
		panel[changeNum] = changePanel[i];
		changePanel[i] = panel[i];
		changePanel[changeNum] = panel[changeNum];
	}
}

void GameStart(int* length) {
	//�p�l���̑傫������
	panel = new int[panel_length*panel_width];
	//����̉摜�\��
	DrawGraph(0, 0, complete_panel, FALSE);
	//3�b��~
	while (GetNowCount() - startTime < 3000) {
		// ���b�Z�[�W
		DrawStringToHandle (90, 80, "���̊G�����낦�Ă�", GetColor(0, 10, 0), text_font01);
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
		//�s�[�X�𓮂������Ƃ��̌��ʉ��Đ�
		PlaySoundFile("button68.mp3", DX_PLAYTYPE_BACK);
	}
}

void GameMain() {
	int length = panel_length * panel_width;

	//�Q�[����
	if (isGame) {
		bool clear = true;

		if (GetMouseInput() && MOUSE_INPUT_LEFT != 0) {
			int x;
			int y;
			GetMousePoint(&x, &y);
			ChangePiece(x / (ScreenSizeX / panel_width), y / (ScreenSizeY / panel_length), &length);
		}


		for (int i = 0; i < length; i++) {
			//�p�Y����������ĂȂ�������
			if (panel[i] != i) {
				clear = false;
			}
			//�p�l���̕\��
			if (panel[i] < length - 1) {
				DrawGraph((i % panel_width) * (ScreenSizeX / panel_width), (i / panel_length) * (ScreenSizeY / panel_length), stage_panel[panel[i]], FALSE);
			}
		}
		//�N���A�V�[����
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

// �I�����
void GameClear() {
	DrawGraph(0, 0, complete_panel, FALSE);
	DrawStringToHandle(130, 110, "�悭�ł��܂���!", GetColor(0, 10, 0), text_font01);

	//��ʃ^�b�`�Ń^�C�g����ʂ�
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

	//�摜�̓ǂ݂���
	LoadDivGraph("Sprites/pic1.png", 9, 3, 3, 200, 200, stage1_pics, FALSE);
	LoadDivGraph("Sprites/pic2.png", 16, 4, 4, 150, 150, stage2_pics, FALSE);
	LoadDivGraph("Sprites/pic3.jpg", 25, 5, 5, 120, 120, stage3_pics, FALSE);
	//�摜�̕\��
	stage1_panel = LoadGraph("Sprites/pic1.png");
	stage2_panel = LoadGraph("Sprites/pic2.png");
	stage3_panel = LoadGraph("Sprites/pic3.jpg");
	title_bg = LoadGraph("Sprites/title.jpg");
	button = LoadGraph("Sprites/button.png");
	//�t�H���g�̍쐬
	text_font01 = CreateFontToHandle("���C���I", 48, 5, DX_FONTTYPE_ANTIALIASING);
	text_font02 = CreateFontToHandle("MS����", 24, 3, DX_FONTTYPE_ANTIALIASING);

	//������
	startTime = 0;
	status = Title;
	//BGM�Đ�
	PlayMusic("c2.mp3", DX_PLAYTYPE_LOOP);

	while (!ProcessMessage()) {
		ClearDrawScreen();
		//�V�[���̏�ԕʂɍX�V
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
