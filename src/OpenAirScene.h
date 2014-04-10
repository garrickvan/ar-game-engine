/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __OPEN_AIR_SCENE_H__
#define __OPEN_AIR_SCENE_H__

#include "iscene.h"
#include "snow.h"
#include "plantmgr.h"
#include "SkyBox.h"
#include "water.h"

/**\brief
 * 游戏主场景类
 */
class OpenAirScene : public IScene
{
public:
	OpenAirScene(void);
	~OpenAirScene(void);


	/**\brief
	 * 场景的初始化
	 */
	BOOL Init(void);


	/**\brief
	 * 从INI文件载入数据
	 */
	BOOL LoadIni(char* _filename,char* _index);


	/**\brief
	 * 更新场景的数据
	 */
	void UpdateData(void);


	/**\brief
	 * 场景的渲染
	 */
	void Render(void);


	CTreeMgr* GetTreeMgr(){return &trees;}

private:
	// 雪的渲染对象
	EGSnow m_cSnow;

	// 天空
	CSkyBox skybox;
	
	// 树
	CTreeMgr trees;

	// 水
	CWater water;
};


#endif
