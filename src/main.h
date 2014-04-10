/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __MAIN_H__
#define __MIAN_H__

#include "stdafx.h"
#include "gameapp.h"


/**\brief
 * 主函数，程序的入口
 */
int APIENTRY WinMain(HINSTANCE hInstance,HINSTANCE hPrevInstance,LPSTR lpCmdLine,int nCmdShow);



/**\brief
 * 窗口消息处理函数
 */
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);



#endif
