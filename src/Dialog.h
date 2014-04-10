/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _CGW_DIALOG_H__
#define _CGW_DIALOG_H__

#include "window.h"
#include "button.h"


/**\brief
 * 对话框类
 */
class Dialog : public EGWindow
{
public:
	Dialog(void);
	~Dialog(void);
	
	/**\brief
	 * 获得按钮数目
	 */
	unsigned int GetButtonNum(){return m_nButtonNum;}
	/**\brief
	 * 获得指定的按钮
	 */
	EGCButton* getButton(unsigned int index)
	{
		if(m_nButtonNum >= index && index >=1)
			return & m_pButton[index - 1];
	}

	/**\brief
	 * 从INI文件载入对话框
	 */
	BOOL LoadIni(char* _filename,char* _index);


private:
	// 菜单上的按钮数目
	unsigned int m_nButtonNum;

	// 按钮指针
	EGCButton*	m_pButton;


	/**\brief
	 * 对话的渲染
	 */
	void OnDraw();
};

#endif
