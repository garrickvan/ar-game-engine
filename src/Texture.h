/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "image.h"


/**\brief
 * 纹理处理类
 */
class EGCTexture
{
public:
	EGCTexture(void);
	~EGCTexture(void);

	unsigned int GetTextureId(int i);

	void SetTexCoord(float u , float v);
	
	void Bind(int i);
	
	BOOL InitTexture(char * filename,BOOL bAlpha=FALSE,BOOL bTga=FALSE);



private:
	void GetImageData(char *filename , BOOL bAlpha , BOOL bTga);
	
	int GetPower(int iSize);
	
	unsigned int tex[3];
	
	int m_iWidth;
	
	int m_iHeight;

	float fScaleWidth;
	
	float fScaleHeight;

	unsigned char *m_pImageData;
	
	GLint color_type;
	
	GLenum color_format;
	
	BOOL bScale;
	
	EGImage *m_pImage;

};





//////////////////////////////////////////////////////////
//--------------------inline函数实现---------------------

inline unsigned int EGCTexture::GetTextureId(int i)
{
	if(i<0)i=0;if(i>2)i=2;
	return tex[i];
}


inline void EGCTexture::SetTexCoord(float u , float v)
{
	u = u * fScaleWidth;
	v = v * fScaleHeight;
	glTexCoord2d(u ,v);
}


inline void EGCTexture::Bind(int i)
{
	if(i<0)i=0;
	if(i>2)i=2;
	glBindTexture(GL_TEXTURE_2D ,tex[i]);
}


#endif
