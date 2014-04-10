/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#include "ARMgr.h"
#include "mathlib.h"
#include "Music.h"

class RecordSkill
{
public:
	RecordSkill(void);
	~RecordSkill(void);

	void RecordSkillFromeCamera();
	
	ARUint8 * returnImg(){return m_RecordImg;};

	void Set(vector3d start,vector3d end);

	BOOL hasRecroded(){return hasRecordSkill;};


	BOOL isRunning(){return IsRunning;};


	BOOL isHit(){return IsHit;};
	
	void init();

	void Chang(vector3d v){if(IsRunning)end_pos = v;};
	
private:

	EGMusic sound;

	ARUint8 * m_RecordImg;

	BOOL hasRecordSkill,IsRunning,IsHit;

	int width,height;

	vector3d start_pos,end_pos;

	float theta;
	
	char skill[3][120][160];

	int GetPiexl(EGSColor &cc);

};

