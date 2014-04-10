/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _BASE_MAGIC_H_
#define _BASE_MAGIC_H_

#include "texture.h"
#include "DrawingBoard.h"
#include "Timer.h"

/** 爆炸火花类 */
class BaseMagic  
{
public:
	BaseMagic();
	~BaseMagic(){}

	BOOL Init(char *filename , float size);
	void SetSpeed(float s){speed = s;}
	void SetColor(float _r,float _g,float _b,float _a){r=_r;g=_g;b=_b;a=_a;}
	void Render();
	void Set(vector3d start , vector3d end);
	void Reset();
	void Change(vector3d& dest){end_pos = dest;}
	BOOL isHit(){return m_isHit;};
	void Update();

private:
	EGCTexture texture;
	EGCDrawingBoard drawingBoard;
	float m_iSize;
	vector3d start_pos,end_pos;
	float speed;
	float r,g,b,a;
	BOOL m_isHit;

	static const float MIN_DISTANCE;
};

#endif
