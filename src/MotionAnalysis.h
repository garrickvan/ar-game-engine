/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _MOTION_ANALYSIS_H_
#define _MOTION_ANALYSIS_H_
#include "stdAfx.h"
#include <AR/gsub.h>
#include <AR/param.h>
#include "Timer.h"
#include "mathlib.h"

struct SkillSelect
{
	char x,y,z;
};

typedef struct SkillSelect SkillSelect;

class MotionAnalysis
{
public:
	MotionAnalysis(void);
	~MotionAnalysis(void);


	/** 更新分析数据 **/
	void update();

	/** 初始函数 **/
	void init(void);

	void getMovePos(vector3d & dis_pos);

	void getSkillSelect(SkillSelect & select);

	vector3d * getMoveMarkCenter(){return &debug;};
	
	BOOL getActiveMove(){return activeMove;};
	
	BOOL getSkillMove(){return skillMove;};
	
	double getMoveTime();

	double getSkillTime();

private:
	
	/** 检测动作激活 **/
	BOOL activeMove;

	BOOL skillMove;
	
	vector3d moveMarkCenter,skillMarkCenter,debug;

	OBJECT_T * objs ;
};
#endif