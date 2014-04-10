/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include "texture.h"
#include "camera.h"


/**\brief
 * 地形类
 */
class EGTerrain
{
public:
	EGTerrain();
	~EGTerrain(){delete [] heightMap; }
	
	BOOL Init(int _width,char* TexFile);
	BOOL Load(char* filename);

	static EGTerrain* GetSysTerrain(){return m_pTerrain;}
	
	/**\brief
	 * 取得地形的宽
	 */
	int GetWidth(){return m_nWidth;}

	int GetCellWidth(){return m_nCellWidth;}

	void Render(void);

	int GetMapWidth(){return m_nWidth * m_nCellWidth;};

	float GetHeight(float x, float z);

private:
	// 地形大小为m_iWidth*m_iWidth，最好是2^n
	unsigned int m_nWidth;

	// 动态高程映射
	short*        heightMap;

	// 每个格子的宽度
	unsigned int m_nCellWidth;

	// 地形上的多重纹理
	EGCTexture terrainTex;

	// 雾的颜色
	float fogColor[4];

	float scanDepth;
	
	static EGTerrain* m_pTerrain;
};


#endif
