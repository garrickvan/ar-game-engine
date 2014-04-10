/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _PROGRESS_BAR_H_
#define _PROGRESS_BAR_H_

#include "StdAfx.h"
#include "Window.h"


/**\brief
 * 进度条类
 */
class EGProgressBar : public EGWindow
{
public:
	EGProgressBar();
	~EGProgressBar(){}


	/**\brief
	 * 从INI文件载入窗口数据
	 */
	virtual BOOL LoadIni(char* filename,char* index);


	/**\brief
	 * 设置进度条的进度
	 */
	void	SetPos(float p){pos=p;}


	/**\brief
	 * 设置进度条边框的宽度
	 */
	void	SetBorderWidth(int b){border=b;}


	/**\brief
	 * 设置进度条的颜色
	 */
	void	SetBarColor(EGSColor c){BarColor=c;}
	

	/**\brief
	 * 获得进度条的进度
	 */
	float	GetPos(){return pos;}
	
	
	/**\brief
	 * 取得边框的宽度
	 */
	int		GetBorderWidth(){return border;}
	
	
	/**\brief
	 * 取得进度条的颜色
	 */
	EGSColor	GetBarColor(){return BarColor;}



private:
	// 用来记录和设置进度条的进度
	float	pos;
	
	//边框宽度
	int		border;
	
	//条的颜色
	EGSColor	BarColor;
	
	
	/**\brief
	 * 绘制进度条
	 *
	 * 虚函数，由其父类EGWindow的draw()来调用
	 */
	virtual void OnDraw(void);
};


#endif
