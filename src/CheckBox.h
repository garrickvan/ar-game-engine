/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _CHECK_BOX_H_
#define _CHECK_BOX_H_

#include "StdAfx.h"
#include "Button.h"


/**\brief
 * 复选框类
 */
class EGCheckBox : public EGCButton  
{
public:
	EGCheckBox(){bChecked=FALSE;}
	~EGCheckBox(){}


	/**\brief
	 * 从INI文件载入
	 */
	virtual BOOL LoadIni(char* filename,char* index);


	/**\brief
	 * 是否被选中
	 */
	BOOL	GetState(){return bChecked;}


	/**\brief
	 * 设置复选框的状态
	 */
	void	SetState(BOOL _bChecked){bChecked=_bChecked;}

private:
	// 是否被选
	BOOL bChecked;


	/**\brief
	 *  绘制
	 */
	virtual void OnDraw(void);

};

#endif
