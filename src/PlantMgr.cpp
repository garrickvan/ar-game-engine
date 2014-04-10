/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "PlantMgr.h"
#include "inifile.h"
#include "Terrain.h"



BOOL CTreeMgr::LoadIni(char* filename,char* index)
{
	EGCIniFile Ini(filename);
	unsigned int t_num=Ini.ReadInt(index,"TreeNum");
	static const int sum_tree_number = t_num * 50;

	//初始化植物
	char* strFile;
	char* strIndex;
	float tree_x,tree_z;
	int map_w = EGTerrain::GetSysTerrain()->GetMapWidth();

	for(unsigned int i=0,j=0; i<sum_tree_number; ++i,++j)
	{
		j %= t_num;
		tree_x = rand() % map_w;
		tree_z = rand() % map_w;

		strFile = Ini.ReadText(index, j);
		strIndex = Ini.ReadData(index,j);
		tree.LoadIni(strFile, strIndex,tree_x,tree_z);
		m_pTrees.push_back(tree);
		_FREE(strFile);
		_FREE(strIndex);
	}

	return TRUE;
}


void CTreeMgr::Render()
{
	glDisable(GL_CULL_FACE);
	for(unsigned int i=0;i<m_pTrees.size();++i)
	{
		m_pTrees[i].Render();
	}
	glEnable(GL_CULL_FACE);
}

