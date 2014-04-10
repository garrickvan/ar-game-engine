/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _SNOW_H_
#define _SNOW_H_

#include "Particle.h"
#include "texture.h"
#include "DrawingBoard.h"
#include "role.h"


/**\brief
 * 雪的渲染类
 */
class EGSnow : public EGParticle  
{
public:
	void Render();
	BOOL Init();
	EGSnow():isBlend(FALSE){}
	~EGSnow(){}

private:
	void ResetPaticle(PARTICLE *particle);
	EGCTexture snow_texture;
	int m_width , m_height , m_depth ,top_of_camera;
	vector3d vCameraPos;
	BOOL isBlend;
	EGCDrawingBoard drawingBoard;
};

#endif
