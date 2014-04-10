/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _BUTTON_H_
#define _BUTTON_H_

#include "StdAfx.h"
#include "Window.h"
#include "Font.h"
#include "IniFile.h"
#include "Sound.h"
#include "Music.h"


// 按钮被按下时要播放的声音文件
#define DOWNL_SOUND_NAME "sound\\click.wav"


/**\brief
 * 按钮类
 */
class EGCButton : public EGWindow
{
public:


public:
	EGCButton();
	virtual ~EGCButton(){}


	/**\brief
	 * 设置按钮上的字
	 */
	void	SetText(char* str){bHasText=TRUE;strcpy(m_chText,str);}


	/**\brief
	 * 取得按钮上的字
	 */
	char*	GetText(){return m_chText;}


	/**\brief
	 * 设置按钮上的字的显示类型
	 */
	void	SetTextStyle(int _style){m_iTextStyle=_style;}


	/**\brief
	 * 取得按钮上的字的显示类型
	 */
	int		GetTextStyle(){return m_iTextStyle;}


	/**\brief
	 * 设置按钮在活动时的颜色
	 */
	void	SetActiveColor(EGSColor c){color2=c;}


	/**\brief
	 * 取得按钮活动时的颜色
	 */
	EGSColor	GetActiveColor(){return color2;}


	/**\brief
	 * 设置按钮平常的颜色
	 */
	void	SetNormalColor(EGSColor _color){color1=_color;}


	/**\brief
	 * 取得按钮活动时的颜色
	 */
	EGSColor	GetNormalColor(){return color1;}


	/**\brief
	 * 设置字体的颜色
	 */
	void	SetTextColor(EGSColor _color){m_sTextColor=_color;}


	/**\brief
	 * 设置按钮是否可用
	 */
	void	SetEnable(BOOL _anable){bEnable=_anable;}


	/**\brief
	 * 取得按钮是否可用
	 */
	BOOL	GetEnable(){return bEnable;}


	/**\brief
	 * 从INI文件载入的按钮数据
	 */
	virtual BOOL LoadIni(char* _filename,char* _index);



private:
	// 按钮上的字
	char	m_chText[256];
	
	// 按钮上是否有字
	BOOL	bHasText;

	// 按钮是否可用
	BOOL	bEnable;

	/**\brief
	 * 绘制循环,虚函数，由其父类EGWindow的draw()来调用
	 */
	virtual void OnDraw(void);


protected:
	// 字类型，可取BTN_TEXT_LEFT,BTN_TEXT_CENTER,BTN_TEXT_RIGHT
	int		m_iTextStyle;

	// 按钮平时的颜色
	EGSColor	color1;

	// 按钮被击活时的颜色
	EGSColor	color2;

	// 字体颜色
	EGSColor	m_sTextColor;
	
	// 鼠标按下时的声音
	static	EGSound m_cSound;

};

#endif
