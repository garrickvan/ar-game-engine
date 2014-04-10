/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __MAP_VIEW_H__
#define __MAP_VIEW_H__

#include "window.h"
#include "texture.h"
#include "MonstersMgr.h"
#include "role.h"

/**\brief
 * 小地图类
 */
class MapView : public EGWindow
{
public:
	MapView(void);
	~MapView(void);

	/**\brief
	 * 从INI文件载入小地图数据
	 */
	BOOL LoadIni(char* _filename,char* _index, char* _videoFilename,char* _videoIndex);

private:
	/**\brief
	 * 小地图的绘制
	 */
	void OnDraw(void);
};


#endif
