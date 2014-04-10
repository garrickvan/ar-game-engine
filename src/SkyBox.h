/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _SKY_BOX_H_
#define _SKY_BOX_H_

#include "camera.h"
#include "texture.h"


/**\brief
 * 天空盒类
 */
class CSkyBox  
{
public:
	CSkyBox();
	~CSkyBox();

	BOOL Init();			// 天空的初始化

	void Render();			// 渲染天空


private:
	EGCTexture texture;		// 天空纹理

	float yrot;				// 围绕Y轴的旋转角度

	vector3d m_sCameraPos;	// 摄像机的位置
	
	GLuint cameraTexture[1];
};

#endif
