/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _ANIMATE_H_
#define _ANIMATE_H_

#include "StdAfx.h"
#include "Texture.h"
#include "IniFile.h"


/**\brief
 * 2D平面动画类
 */
class EGAnimate  
{
public:
	// 动画帧与帧之间的延时
	float	DeltaTime;

	// 颜色
	EGSColor	color;

	// 所在矩形
	EGSRect	rect;


	EGAnimate();
	~EGAnimate(){}


	/**\brief
	 * 是否透明处理
	 */
	BOOL GetBlendState(){return bBlend;}


	/**\brief
	 * 载入纹理
	 *
	 *\param	filename	纹理文件名
	 *\param	nFrameNum	总帧数
	 *\param	_bBlend		是否透明处理
	 *\return	BOOL		是否成功载入纹理
	 */
	BOOL LoadTexture(char* filename,int nFrameNum,BOOL _bBlend);


	/**\brief
	 * 通过INI文件初始化
	 */
	virtual BOOL LoadIni(char* filename,char* index);
	

	/**\brief
	 * 渲染动画
	 */
	void draw(void);


	/**\brief
	 * 移动动画
	 *
	 *\param	_dx	x上偏移
	 *\param	_dx	y上偏移
	 */
	void move(int _dx,int _dy){rect.x+=_dx;rect.y+=_dy;}



private:
	// 动画的帧数
	unsigned int FrameNum;
	
	// 是否透明处理
	BOOL		bBlend;

	// 动画所用的纹理
	EGCTexture	texture;
	
	// 正在播放第n帧（从0开始）
	unsigned int n;

	// 两帧间的间隔时间
	float		m_fDeltaTime;
};


#endif
