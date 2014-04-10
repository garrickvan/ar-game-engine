/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "resource.h"
#include "main.h"

//////////////////////////////////////////////////////////
//-----------------下面是全局变量的定义-------------------
// 窗口信息结构
EGSWindowInfo* g_sWindowInfo;

// 应用程序
GameApp*	 g_cGameApp;



//////////////////////////////////////////////////////////
//-----------------下面是全局函数的实现-------------------

/**\brief
 * 主函数，程序的入口
 */
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow)
{
	/*设置随机种子*/
	srand(36);

	g_cGameApp		= new GameApp();
	g_sWindowInfo	= new EGSWindowInfo;

	if(!g_sWindowInfo->LoadIni("ini\\init.ini","window"))
	{
		_ERROR("窗口初始化失败!");
		_DELETE(g_cGameApp);
		return 0;
	}

	if(!g_cGameApp->Init(*g_sWindowInfo))
	{
		_ERROR("程序初始化失败!");
		_DELETE(g_cGameApp);
		return 0;
	}

	g_cGameApp->Run();

	_DELETE(g_cGameApp);
	_DELETE(g_sWindowInfo);

	return 0;
}



/**\brief
 * 窗口消息处理函数
 */
LRESULT CALLBACK WndProc(HWND hWnd,UINT message,WPARAM wParam,LPARAM lParam)
{
	return g_cGameApp->MessageHandler(hWnd,message,wParam,lParam);
}

