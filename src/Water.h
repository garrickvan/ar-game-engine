/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _WATER_H_
#define _WATER_H_

#include "texture.h"


/**\brief
 * 水的渲染类
 */
class CWater  
{
public:
	CWater();
	~CWater();

	/**\brief
	 * 初始化,主要是纹理的载入
	 */
	BOOL Init();
	

	/**\brief
	 *  水的渲染
	 */
	void Render();



private:
	// 纹理
	EGCTexture texture;
	
	// 纹理移动的最小单位
	float delta;

};

#endif
