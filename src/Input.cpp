#include "stdafx.h"
#include "Input.h"


CKeyboard::CKeyboard(LPDIRECTINPUT8 pDI, HWND hwnd)
{
	if (FAILED(pDI->CreateDevice(GUID_SysKeyboard, &m_pDIDev, NULL)))
	{
		_ERROR("Failed to set data format<keyboard>");
	}

	if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIKeyboard)))
	{
		_ERROR("Failed to set data format<keyboard>");
	}

	if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
	{
		_ERROR("Failed to set cooperative level<keyboard>");
	}

	if (FAILED(m_pDIDev->Acquire()))
	{
		//_ERROR("Failed to acquire<keyboard>");
	}

	Clear();
}

CKeyboard::~CKeyboard()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}


/**\brief
 * 更新键盘数据，放在主循环中
 */
BOOL CKeyboard::Update()
{
  if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys), (LPVOID)m_keys)))
  {
		if (FAILED(m_pDIDev->Acquire()))return FALSE;

		if (FAILED(m_pDIDev->GetDeviceState(sizeof(m_keys),(LPVOID)m_keys)))return FALSE;
	}

	return TRUE;
}

////////////////////////////////////////////////
////////////////////////////////////////////////

CMouse::CMouse(LPDIRECTINPUT8 pDI, HWND hwnd, BOOL isExclusive)
{
	RECT r;
	GetClientRect(hwnd , &r);

	m_iWidth = r.right - r.left;
	m_iHeight = r.bottom - r.top;

	m_sMousePos.x=m_iWidth/2;
	m_sMousePos.y=m_iHeight/2;

	if (FAILED(pDI->CreateDevice(GUID_SysMouse, &m_pDIDev, NULL)))
	{
		_ERROR("Failed to create mouse device");
	}

	if (FAILED(m_pDIDev->SetDataFormat(&c_dfDIMouse)))
	{
		_ERROR("Failed to set data format<mouse>");
	}

	DWORD flags;

	if (isExclusive)
		flags = DISCL_FOREGROUND|DISCL_EXCLUSIVE|DISCL_NOWINKEY;
	else
		flags = DISCL_FOREGROUND|DISCL_NONEXCLUSIVE;


	if (FAILED(m_pDIDev->SetCooperativeLevel(hwnd, flags)))
	{
		_ERROR("Failed to set cooperative level<mouse>");
	}

	if (FAILED(m_pDIDev->Acquire()))
	{
		//_ERROR("Failed to acquire<mouse>");
	}

	if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
	{
		//_ERROR("Failed to get device state<mouse>");
	}
}


CMouse::~CMouse()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}


/**\brief
 * 更新鼠标参数
 */
BOOL CMouse::Update()
{
	if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
	{
		if (FAILED(m_pDIDev->Acquire()))
		{
			return FALSE;
		}
		if (FAILED(m_pDIDev->GetDeviceState(sizeof(DIMOUSESTATE), &m_state)))
		{
			return FALSE;
		}
	}
 
	m_sMousePos.x+=m_state.lX;
	m_sMousePos.y-=m_state.lY;
	

	/*char str[80];
	sprintf(str, "the x is %d the y is %d .", m_state.lX, m_state.lY);
	ShowMessage(str);*/

	if(m_sMousePos.x<0)m_sMousePos.x=0;
	if(m_sMousePos.x>m_iWidth)m_sMousePos.x=m_iWidth;
	if(m_sMousePos.y<0)m_sMousePos.y=0;
	if(m_sMousePos.y>m_iHeight)m_sMousePos.y=m_iHeight;

  return TRUE;
}



///////////////////////////////////////////
///////////////////////////////////////////

CJoystick::CJoystick(LPDIRECTINPUT8 pDI, HINSTANCE appInstance)
{
}

CJoystick::~CJoystick()
{
	if (m_pDIDev)
	{
		m_pDIDev->Unacquire();
		m_pDIDev->Release();
	}
}



//////////////////////////////////
//////////////////////////////////
EGInput* EGInput::m_pInput=NULL;
HWND	EGInput::m_hWnd=NULL;

/**\brief
 * 初始化输入设备
 */
BOOL EGInput::Init(HWND hwnd,HINSTANCE appInstance,BOOL isExclusive,DWORD flags)
{
	m_pKeyboard = NULL; 
	m_pMouse = NULL; 
	m_pJoystick = NULL; 

	m_hWnd=hwnd;

	if (FAILED(DirectInput8Create(appInstance,
								DIRECTINPUT_VERSION,
								IID_IDirectInput8,
								(void **)&m_pDI,NULL)))
		return FALSE;

	if (flags & IS_USEKEYBOARD)
	{
		m_pKeyboard = new CKeyboard(m_pDI, hwnd);
	}
	if (flags & IS_USEMOUSE)
	{
		m_pMouse = new CMouse(m_pDI, hwnd, TRUE);
	}
	if (flags & IS_USEJOYSTICK)
	{
		m_pJoystick = new CJoystick(m_pDI, appInstance);
	}
	
	return TRUE;
}


/**\brief
 * 获得光标的3D坐标
 */
vector3d EGInput::GetMousePosition()
{
	float depth[2];
	EGSPoint2 p;
	p=m_pMouse->GetMousePos();

	GLdouble x,y,z;
	glReadPixels(p.x,p.y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,depth);
	GLdouble model[16],project[16];
	glGetDoublev(GL_MODELVIEW_MATRIX,model);
	glGetDoublev(GL_PROJECTION_MATRIX,project);
	GLint viewport[4]={0,0,m_pMouse->m_iWidth,m_pMouse->m_iHeight};
	gluUnProject(p.x,p.y,depth[0],model,project,viewport,&x,&y,&z);

	return vector3d((float)x,(float)y,(float)z);
}


/**\brief
 * 更新输入设备的参数
 */
void EGInput::Update()
{
	if (m_pKeyboard)	m_pKeyboard->Update();
	if (m_pMouse)		m_pMouse->Update();
	if (m_pJoystick)	m_pJoystick->Update();


	//-----用来记录Clicked事件------
	static BOOL bMouseLDown=FALSE;
	if(m_pMouse->ButtonDown(0)==TRUE)
	{
		bMouseLDown=TRUE;
		bClick=FALSE;
	}
	else if(bMouseLDown==TRUE&&m_pMouse->ButtonDown(0)==FALSE)
	{
		bMouseLDown=FALSE;
		bClick=TRUE;
	}
	else
	{
		bMouseLDown=FALSE;
		bClick=FALSE;
	}
	//------------------------------

	//--用来记录FirstMouseDownL事件--
	static BOOL bMouseLUp=TRUE;
	if(m_pMouse->ButtonDown(0)==FALSE)bMouseLUp=TRUE;
	if(m_pMouse->ButtonDown(0)==TRUE&&bMouseLUp==TRUE)
	{
		bFirstMDownL=TRUE;
		bMouseLUp=FALSE;
	}
	else bFirstMDownL=FALSE;
	//-------------------------------
	
}


/**\brief
 * 释放输入设备资源
 */
BOOL EGInput::Shutdown()
{
  UnacquireAll();

  if (m_pKeyboard)_DELETE(m_pKeyboard);
  if (m_pKeyboard)_DELETE(m_pMouse);
  if (m_pJoystick)_DELETE(m_pJoystick);
  if (FAILED(m_pDI->Release()))return FALSE;

  return TRUE;
}

