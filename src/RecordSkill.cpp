/**
 *作者：范修盛
 *2011年10月30日
 */
#include "RecordSkill.h"

RecordSkill::RecordSkill(void)
	:m_RecordImg(NULL),hasRecordSkill(FALSE),IsRunning(FALSE),IsHit(FALSE)
{
	height = SYS_VIDEO->getHeight() / 4;
	width = SYS_VIDEO->getWidth() / 4;
	m_RecordImg = NULL;
	memset(skill,0,3*160*120*sizeof(char));
}

RecordSkill::~RecordSkill(void)
{
	if(m_RecordImg) delete m_RecordImg;
}

void RecordSkill::init()
{
	//sound.bPlay();
}

 /**\brief
  *录制技能
  */
 void RecordSkill::RecordSkillFromeCamera()
 {
	ARUint8 * m_Img = SYS_VIDEO->getImgFromCap();
	height = SYS_VIDEO->getHeight();
	width = SYS_VIDEO->getWidth();
	int selct;
	EGSColor cc;

	if(m_Img)
	{
		long sum = height * width * 4;
		m_RecordImg = (ARUint8 *) malloc(sizeof(ARUint8) * sum);
		gluScaleImage(GL_BGRA,width, height, GL_UNSIGNED_BYTE, m_Img, width, height, GL_UNSIGNED_BYTE, m_RecordImg);
		hasRecordSkill = TRUE;
	}
 }