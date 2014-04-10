/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _STATIC_OBJ_MGR_H_
#define _STATIC_OBJ_MGR_H_

#include "StaticObj.h"
#include "terrain.h"
#include <vector>
using namespace std;


/**\brief
 * 静态实体对象管理类
 */
class EGStaticObjMgr
{
public:
	EGStaticObjMgr();
	~EGStaticObjMgr();


	/**\brief
	 * 获得对象的个数
	 */
	unsigned int GetObjNum(){return m_nObjNum;}
	

	/**\brief
	 * 点选操作中用的渲染
	 */
	void Render_Pick(int id);
	
	
	/**\brief
	 * 渲染所有静态对象实体
	 */
	void Render();
	

	/**\brief
	 * 初始化
	 */
	BOOL LoadIni(char* filename,char* index);


	static EGStaticObjMgr* GetSysStaticObjMgr(void){return m_pStaticObjMgr;}


	StaticObj* GetStaticObj(unsigned int n){if(n>=m_nObjNum)return NULL;return m_pObjArray+n;}
private:
	// MS3D文件操作
	EGMS3DFile ms3d;

	//
	unsigned int m_nObjNum;

	// 
	StaticObj* m_pObjArray;

	static EGStaticObjMgr* m_pStaticObjMgr;
};


////////////////////////////////////////////////////////
//--------------------inline函数实现--------------------
/**\brief
 * 点选操作中用的渲染
 */
inline void EGStaticObjMgr::Render_Pick(int id)
{
	(m_pObjArray+id)->Render();
}



/**\brief
 * 渲染所有静态对象实体
 */
inline void EGStaticObjMgr::Render()
{
	for(unsigned int i=0;i<m_nObjNum;++i)
	{
		(m_pObjArray+i)->Render();
	}
}

#endif
