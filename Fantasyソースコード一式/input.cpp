#include "main.h"
#include "input.h"


BYTE Input::m_OldKeyState[256];
BYTE Input::m_KeyState[256];

BYTE Input::m_OldMouseState[3];
BYTE Input::m_MouseState[3];

long Input::m_MouseX = 0;
long Input::m_MouseY = 0;
long Input::m_MouseWheelDelta = 0;


void Input::Init()
{

	memset(m_OldKeyState, 0, 256);
	memset(m_KeyState, 0, 256);

	memset(m_OldMouseState, 0, sizeof(m_OldMouseState));
	memset(m_MouseState, 0, sizeof(m_MouseState));

}

void Input::Uninit()
{

}

void Input::Update()
{

	memcpy(m_OldKeyState, m_KeyState, 256);
	GetKeyboardState(m_KeyState);

	// マウスの状態を更新
	m_OldMouseState[0] = m_MouseState[0];
	m_OldMouseState[1] = m_MouseState[1];
	m_OldMouseState[2] = m_MouseState[2];

	m_MouseState[0] = (BYTE)(GetAsyncKeyState(VK_LBUTTON) & 0x8000 ? 1 : 0);
	m_MouseState[1] = (BYTE)(GetAsyncKeyState(VK_RBUTTON) & 0x8000 ? 1 : 0);
	m_MouseState[2] = (BYTE)(GetAsyncKeyState(VK_MBUTTON) & 0x8000 ? 1 : 0);

	POINT mousePos;
	GetCursorPos(&mousePos);
	ScreenToClient(GetWindow(), &mousePos); // GetWindow()はメインループなどでウィンドウハンドルを取得する関数を想定
	m_MouseX = mousePos.x;
	m_MouseY = mousePos.y;

	// 適切な場所でリセットするようにする
	m_MouseWheelDelta = 0; // 各フレームでリセットされるようにする

}

bool Input::GetKeyPress(BYTE KeyCode)
{
	return (m_KeyState[KeyCode] & 0x80);
}

bool Input::GetKeyTrigger(BYTE KeyCode)
{
	return ((m_KeyState[KeyCode] & 0x80) && !(m_OldKeyState[KeyCode] & 0x80));
}

bool Input::IsMousePressed(int button)
{
	if (button < 0 || button >= 3) return false;
	return m_MouseState[button] == 1;
}

