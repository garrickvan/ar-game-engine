/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef __EDIT_H__
#define __EDIT_H__

#include "stdafx.h"
#include "window.h"
#include "ime.h"
#include "font.h"


/**\brief
 * edit编辑框类
 */
class Edit : public EGWindow
{
public:
	Edit(void);
	~Edit(void){}


	/**\brief
	 * 是否还能输入字符
	 */
	BOOL IsFocus(){return m_bFocus;}


	/**\brief
	 * 设置焦点
	 */
	void SetFocus(BOOL _focus){m_bFocus=_focus;}


	/**\brief
	 * 取得编辑框中的字符串
	 */
	char* GetText(){return m_szText;}


	/**\brief
	 * 是否输入了回车
	 */
	BOOL GetReturnState(){return m_bReturn;}
	void SetReturnState(BOOL _b){m_bReturn=_b;}


	/**\brief
	 * 设置显示输入法名称的位置
	 */
	void SetInputNamePos(EGSPoint2 _point){m_sNamePos=_point;}


	/**\brief
	 * 设置字体颜色
	 */
	void SetFontColor(EGSColor _color){m_sFontColor=_color;}


	/**\brief
	 * 取得字体颜色
	 */
	EGSColor GetFontColor(){return m_sFontColor;}


	/**\brief
	 * 设置插入符颜色
	 */
	void SetCaretColor(EGSColor _color){m_sCaretColor=_color;}


	/**\brief
	 * 取得插入符颜色
	 */
	EGSColor GetCaretColor(){return m_sCaretColor;}


	/**\brief
	 * 设置是否为密码输入
	 */
	void SetPasswordState(BOOL _b){m_bPassword=_b;}


private:
	/**\brief
	 * 绘制控件
	 */
	virtual void OnDraw(void);


	// 是否还能输入
	BOOL	m_bFocus;

	// 是否按了回车
	BOOL	m_bReturn;

	// 是否是密码输入
	BOOL	m_bPassword;

	// 输入的字符串
	char	m_szText[256];

	// 输入法名显示的位置
	EGSPoint2	m_sNamePos;

	// 插入点的位置
	int		m_iCaret;

	// 字体颜色
	EGSColor	m_sFontColor;

	// 插入符颜色
	EGSColor	m_sCaretColor;

	// 一个中间变量，用在插入符的闪烁上
	float	m_fDeltaTime;
};


#endif
