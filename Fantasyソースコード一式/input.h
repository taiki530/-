#pragma once

#pragma once

#include <Windows.h>

// マウスボタンの定義
#define MOUSE_LEFT 0
#define MOUSE_RIGHT 1
#define MOUSE_MIDDLE 2


class Input
{
private:
	static BYTE m_OldKeyState[256];
	static BYTE m_KeyState[256];

	static BYTE m_OldMouseState[3]; // 0:左, 1:右, 2:中央
	static BYTE m_MouseState[3];    // 0:左, 1:右, 2:中央

	static long m_MouseX;
	static long m_MouseY;
	static long m_MouseWheelDelta;


public:
	static void Init();
	static void Uninit();
	static void Update();

	static bool GetKeyPress(BYTE KeyCode);
	static bool GetKeyTrigger(BYTE KeyCode);

	// マウス関連の関数
	static bool IsMousePressed(int button);
	static long GetMouseX() { return m_MouseX; }
	static long GetMouseY() { return m_MouseY; }
	static long GetMouseWheelDelta() { return m_MouseWheelDelta; } // マウスホイールのデルタ値を取得
	static void SetMouseWheelDelta(long delta) { m_MouseWheelDelta = delta; } // マウスホイールのデルタ値を設定

};
