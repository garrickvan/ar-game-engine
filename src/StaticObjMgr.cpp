/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "StaticObjMgr.h"
#include "IniFile.h"


EGStaticObjMgr* EGStaticObjMgr::m_pStaticObjMgr=NULL;

EGStaticObjMgr::EGStaticObjMgr()
{
	m_pStaticObjMgr=this;
}

EGStaticObjMgr::~EGStaticObjMgr()
{
	_DELETE_ARRAY(m_pObjArray);
}


/**\brief
 * 初始化
 */
BOOL EGStaticObjMgr::LoadIni(char* filename,char* index)
{
	EGCIniFile Ini(filename);
	m_nObjNum=Ini.ReadInt(index,"ObjNum");
	m_pObjArray=new StaticObj[m_nObjNum];

	//初始化模型
	char* strFile;
	char* strIndex;
	for(unsigned int i=0; i<m_nObjNum; ++i)
	{
		strFile = Ini.ReadText(index, i);
		strIndex = Ini.ReadData(index,i);
		m_pObjArray[i].LoadIni(strFile, strIndex);
		_FREE(strFile);
		_FREE(strIndex);
	}

	return TRUE;
}

