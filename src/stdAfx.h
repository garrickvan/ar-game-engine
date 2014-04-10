/**
 *作者：范修盛
 *2011年10月30日
 */
/**  
*   @file   stdafx.h
*/
#pragma once
#include <afxmt.h>
#ifndef _STDAFX_H_
#define _STDAFX_H_

#define WIN32_LEAN_AND_MEAN	

/** 为处理WM_MOUSEWHEEL消息而加的 */
#define _WIN32_WINDOWS 0x0400

/** 包含统中的头文件 */
#include <stdlib.h>
#include <stdio.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
#include <iostream>

/** 包含OpenGL头文件 */
#include "gl/glut.h"
//#include <gl\glaux.h>
//#include <gl\glext.h>

/** 包含自定义头文件 */
#include "general.h"
#include "structdef.h"


/** 要用到的链接库 */
#pragma comment(lib, "opengl32.lib")		
#pragma comment(lib, "glut32.lib")							
#pragma comment(lib, "glaux.lib")	
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dsound.lib")
#pragma comment(lib,"winmm.lib")
#pragma comment(lib,"XInput.lib")
//////////////AR依赖库////////////
#pragma comment(lib,"libAR.lib")
#pragma comment(lib,"libARvideo.lib")
#pragma comment(lib,"libARgsub.lib")

/** 禁止编译器出现类型转换的警告 */
#pragma warning(disable: 4311)                                 
#pragma warning(disable: 4312)
#pragma warning(disable: 4244)
#pragma warning(disable: 4018)
#pragma warning(disable: 4267)
#pragma warning(disable: 4996)

#endif