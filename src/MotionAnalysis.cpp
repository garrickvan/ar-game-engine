/**
 *作者：范修盛
 *2011年10月30日
 */
#include "MotionAnalysis.h"
#include "ARMgr.h"

MotionAnalysis::MotionAnalysis(void)
	:activeMove(FALSE),objs(NULL),skillMove(FALSE)
{}

MotionAnalysis::~MotionAnalysis(void)
{
}

void MotionAnalysis::init(void)
{
	/** 卡片对象 **/
	objs = SYS_VIDEO->getObjects();
}
	/** 移动卡片消逝时间 **/
static double		moveMarkElapsedTime = 0.0f;
	/** 技能选择卡片消逝时间 **/
static double		skillMarkElapsedTime = 0.0f;
	
double MotionAnalysis::getMoveTime(){return moveMarkElapsedTime;}
double MotionAnalysis::getSkillTime(){return skillMarkElapsedTime;}

/** 分析当前图片 **/
void MotionAnalysis::update()
{
	static BOOL			MarkMoveCenter = FALSE;
	static BOOL			MarkSkillCenter = FALSE;

	moveMarkElapsedTime += SYS_TIMER->m_fTime;
	skillMarkElapsedTime += SYS_TIMER->m_fTime;

	/** 移动卡片的标定及解标定 **/
	if(objs[0].visible != -1)
	{
		if(!activeMove && !MarkMoveCenter) //判断第一并标定移动中心
		{
			MarkMoveCenter = TRUE;
			moveMarkElapsedTime = 0.0f;
		}

		if(MarkMoveCenter)
		{
			if(moveMarkElapsedTime >= .5f) //标定中心
			{
				moveMarkCenter.x = objs[0].trans[0][3];
				moveMarkCenter.z = objs[0].trans[2][3];
				MarkMoveCenter = FALSE;
			}
		}
		else
		{
			moveMarkElapsedTime = 0.0f;
		}
		activeMove = TRUE;
	}
	else
	{
		if(moveMarkElapsedTime >= 10.0f) //消逝5秒钟表明卡片移出摄像头视野,重置参数
			MarkMoveCenter = FALSE;
		if(moveMarkElapsedTime >= .5f)
			activeMove = FALSE;
	}

	/** 技能卡片的标定及解标定 **/
	if(objs[1].visible != -1)
	{
		if(!skillMove && !MarkSkillCenter) //判断第一并标定移动中心
		{
			MarkSkillCenter = TRUE;
			skillMarkElapsedTime = 0.0f;
		}

		if(MarkSkillCenter)
		{
			if(skillMarkElapsedTime >= .5f) //标定中心
			{
				skillMarkCenter.x = objs[1].trans[0][3];
				skillMarkCenter.y = objs[1].trans[1][3];
				skillMarkCenter.z = objs[1].trans[2][3];
				MarkSkillCenter = FALSE;
			}
		}
		else
		{
			skillMarkElapsedTime = 0.0f;
		}
		skillMove = TRUE;
	}
	else
	{
		if(skillMarkElapsedTime >= 10.0f) //消逝5秒钟表明卡片移出摄像头视野,重置参数
			MarkSkillCenter = FALSE;
		if(skillMarkElapsedTime >= .5f)
			skillMove = FALSE;
	}
}

/** 分析移动卡片获取移动方向 **/
 void MotionAnalysis::getMovePos(vector3d & dis_pos)
{
	vector3d		last_pos;
	float x,z;

	
	if(objs[0].visible != -1)
	{
		last_pos.x = objs[0].trans[0][3];
		last_pos.z = objs[0].trans[2][3];
	}
	else
		return;
	
	x = last_pos.x - moveMarkCenter.x;
	z = last_pos.z - moveMarkCenter.z;
	
	if(x >= 55.0f)	dis_pos.x -= 10.0f;
	if(x <= -55.0f)	dis_pos.x += 10.0f;
	if(z >= 120.0f)	dis_pos.z += 10.0f;
	if(z <= -120.0f)	dis_pos.z -= 10.0f;
}

 /** 分析移动卡片获取技能选择表 **/
 void MotionAnalysis::getSkillSelect(SkillSelect & select)
{
	vector3d		last_pos;
	float x,y,z;

	select.x = 0;
	select.y = 0;
	select.z = 0;

	debug.x = 0.f;
	debug.y = 0.f;
	debug.z = 0.f;
	
	if(objs[1].visible != -1)
	{
		last_pos.x = objs[1].trans[0][3];
		last_pos.y = objs[1].trans[1][3];
		last_pos.z = objs[1].trans[2][3];
	}
	else
		return;
	
	x = last_pos.x - skillMarkCenter.x;
	y = last_pos.y - skillMarkCenter.y;
	z = last_pos.z - skillMarkCenter.z;
	debug.x = x;
	debug.y = y;
	debug.z = z;

	if(fabs(x) > fabs(y))
	{
		if(x >= 55.0f)		select.x = 2;
		if(x <= -55.0f)		select.x = 1;
	}

	if(fabs(x) < fabs(y))
	{
		if(y >= 55.0f)		select.y = 2;
		if(y <= -55.0f)		select.y = 1;
	}

	if(z <= -120.0f)
		select.z = 1;
	else
		select.z = 0;
}