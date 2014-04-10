/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _MONSTER_H_
#define _MONSTER_H_

#include "ActiveObj.h"
#include "terrain.h"
#include "MagicSkillMgr.h"
#include "info.h"
#include "sound.h"

extern CInfo ObjectInfo;


/**\brief
 * 游戏中的怪物
 */
class EGMonster : public EGActiveObj
{
public:
	EGMonster(void);
	~EGMonster(void){_DELETE(property);}

	/**\brief
	 * 设置速度
	 */
	void SetSpeed(float s);
	void Render_Pick(void);

	/**\brief
	 * 设置怪物的属性
	 */
	void SetMonsterInfo(SActiveObjPro& info);
	void SetShadow(BOOL b){property->shadow=b;}

	/**\brief
	 * 渲染怪物
	 */
	virtual void OnRender(void){EGActiveObj::OnRender();m_magicSkillMgr.Render();}

	/**\brief
	 * 渲染后的数据更新
	 */
	virtual void EndRender(void);


	/**\brief
	 * 初始化一个怪物
	 */
	BOOL Init(char* modelname,char* texturename,float scale,float h);


	/**\brief
	 * 从INI文件初始化一个怪物
	 */
	BOOL LoadIni(char* filename,char* index);


private:
	// 魔法技能管理器
	MagicSkillMgr m_magicSkillMgr;

	int time_elapse;

	EGSound sound;

	BOOL isDead;

	BOOL tracking;
};

#endif
