/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "aviscene.h"
#include "inifile.h"

AVIScene::AVIScene(void):m_strAVIFile(NULL)
{
}

AVIScene::~AVIScene(void)
{
	m_cAVI.Stop();
	_DELETE(m_strAVIFile);
}


/**\brief
 * 场景的初始化
 */
BOOL AVIScene::Init(void)
{
	if(!m_cAVI.Init(EGInput::GetSysHWnd()))return FALSE;

	return TRUE;
}


/**\brief
 * 从INI文件载入数据
 */
BOOL AVIScene::LoadIni(char* _filename,char* _index)
{
	EGCIniFile Ini(_filename);
	m_strAVIFile=Ini.ReadText(_index,"AVIFile");

	return TRUE;
}


/**\brief
 * 更新场景数据
 */
void AVIScene::UpdateData(void)
{
}


/**\brief
 * 场景的渲染
 */
void AVIScene::Render(void)
{
	if(!m_strAVIFile)return;

	m_cAVI.Load(m_strAVIFile);
	m_cAVI.Play();
	if(SYS_KEYBOARD->KeyDown(DIK_ESCAPE)||SYS_MOUSE->ButtonDown(1))m_cAVI.Stop();

}
