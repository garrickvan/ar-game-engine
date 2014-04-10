#include "stdafx.h"
#include "openairscene.h"

OpenAirScene::OpenAirScene(void)
{
}

OpenAirScene::~OpenAirScene(void)
{
}

/**\brief
 * 场景的初始化
 */
BOOL OpenAirScene::Init(void)
{
	if(!m_cSnow.Init())	return FALSE;
	if(!trees.LoadIni("ini/plants.ini","plants"))	return FALSE;
	if(!skybox.Init())	return FALSE;
	if(!water.Init())	return FALSE;

	return TRUE;
}


/**\brief
 * 从INI文件载入数据
 */
BOOL OpenAirScene::LoadIni(char* _filename,char* _index)
{
	return TRUE;
}


/**\brief
 * 更新场景的数据
 */
void OpenAirScene::UpdateData(void)
{
}


/**\brief
 * 场景的渲染
 */
void OpenAirScene::Render(void)
{	
	water.Render();
	skybox.Render();
	m_cSnow.Render();
	trees.Render();
}
