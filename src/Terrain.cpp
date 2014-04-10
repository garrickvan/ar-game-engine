/**
 *作者：范修盛
 *2011年10月30日
 */
#include "stdafx.h"
#include "terrain.h"
#include "IniFile.h"
#include "math.h"
#include "Monster.h"

const static int stock_height = 5.0f;

EGTerrain Terrain;

EGTerrain* EGTerrain::m_pTerrain = NULL;

EGTerrain::EGTerrain()
{
	m_nWidth = 64.0;
	scanDepth = 16.0;
	m_nCellWidth = 32;
	fogColor[0] = 0.75f;
	fogColor[1] = 0.9f;
	fogColor[2] = 1.0f;
	fogColor[3] = 1.0f;
	m_pTerrain = this;
}

BOOL EGTerrain::Init(int _width,char* TexFile)
{
	m_nWidth=_width;

	// 生成地形
	heightMap = new short[(m_nWidth+1)*(m_nWidth+1)];

	for(unsigned int j=0;j<=m_nWidth;++j)
		for(unsigned int i=0;i<=m_nWidth;++i)
	{
		heightMap[i*m_nWidth+j]=0;
		//heightMap[i*m_nWidth+j]=20*sin(j)*cos(i*i);
	}

	// 装入纹理
	terrainTex.InitTexture(TexFile,TRUE);

	return TRUE;
}

BOOL EGTerrain::Load(char* filename)
{
	FILE* fp;
	fp=fopen(filename,"rb");
	if(fp==NULL)return FALSE;
	fread(&m_nWidth,sizeof(unsigned int),1,fp);
	if(m_nWidth<=0)return FALSE;
	fread(&m_nCellWidth,sizeof(unsigned int),1,fp);
	if(m_nCellWidth<=0)return FALSE;

	heightMap=new short[(m_nWidth+1)*(m_nWidth+1)];
	fread(heightMap,sizeof(short),(m_nWidth+1)*(m_nWidth+1),fp);

	char t_chTexFile[20];
	strcpy(t_chTexFile,"scene/");
	char str[15];
	fread(str,sizeof(char),15,fp);
	strcat(t_chTexFile,str);
	terrainTex.InitTexture(t_chTexFile,TRUE);

	fclose(fp);
	return TRUE;
}

float EGTerrain::GetHeight(float x, float z)
{
	float projCameraX, projCameraZ;

	projCameraX = x / m_nCellWidth;
	projCameraZ = z / m_nCellWidth;
	// 计算高程坐标(Col0, Row0)，(Col1, Row1)
	int col0 = int(projCameraX);
	int row0 = int(projCameraZ);
	unsigned int col1 = col0 + 1;
	unsigned int row1 = row0 + 1;
	// 确保单元坐标不超过高程以外
	if (col1 > m_nWidth)col1 = 0;
	if (row1 > m_nWidth)row1 = 0;

	// 获取单元的四个角的高度
	float h00 = (float)(heightMap[col0 + row0*m_nWidth]);
	float h01 = (float)(heightMap[col1 + row0*m_nWidth]);
	float h11 = (float)(heightMap[col1 + row1*m_nWidth]);
	float h10 = (float)(heightMap[col0 + row1*m_nWidth]);

	// 计算机摄像机相对于单元格的位置
	float tx = projCameraX - float(col0);
	float ty = projCameraZ - float(row0);

	// 下一步是进行双线性插值
	float txty = tx * ty;

	float final_height	= h00 * (1.0f - ty - tx + txty)
						+ h01 * (tx - txty)
						+ h11 * txty
						+ h10 * (ty - txty);
	return final_height;
}

void EGTerrain::Render(void)
{
	static int obstructz1 = DISTANCE_TO_Y_SIDE / m_nCellWidth;
	static int obstructx1 = DISTANCE_TO_X_SIDE / m_nCellWidth;
	static int obstructz2 = (GetMapWidth() - DISTANCE_TO_Y_SIDE) / m_nCellWidth;
	static int obstructx2 = (GetMapWidth() - DISTANCE_TO_X_SIDE) / m_nCellWidth;
	static float escapeTime = 0.0f,alpha = 0.1f,offset = 0.08f;

	escapeTime += SYS_TIMER->m_fTime;

	int z, x;

	vector3d camera_pos;
	EGCamera::GetCameraPointer()->GetCameraPos(&camera_pos);

	glFogi(GL_FOG_MODE, GL_LINEAR);
	glFogfv(GL_FOG_COLOR, fogColor);
	glFogf(GL_FOG_START, scanDepth * 0.4f);
	glFogf(GL_FOG_END, scanDepth * 40.f);
	glHint(GL_FOG_HINT, GL_FASTEST);
	glEnable(GL_FOG);

	glEnable(GL_TEXTURE_2D);
	glColor3f(1.f,1.f,1.f);

	float dist_to_role=EGCamera::GetCameraPointer()->GetDestToRole();
	for(z=(int)((camera_pos.z-dist_to_role)/m_nCellWidth-scanDepth),z=z<0?0:z;
		(z<(camera_pos.z-dist_to_role)/m_nCellWidth+scanDepth)&&(z<m_nWidth);
		++z)
	{
		terrainTex.Bind(2);
		glBegin(GL_TRIANGLE_STRIP);
		for(x=(int)(camera_pos.x/m_nCellWidth-scanDepth),x=x<0?0:x;(x<camera_pos.x/m_nCellWidth+scanDepth)&&x<m_nWidth;++x)
		{
			glTexCoord2f((float)x/(float)m_nWidth,(float)z/(float)m_nWidth);
			glVertex3i(x*m_nCellWidth,		heightMap[x+z*m_nWidth],		z*m_nCellWidth);

			glTexCoord2f((float)x/(float)m_nWidth,(float)(z+1)/(float)m_nWidth);
			glVertex3i(x*m_nCellWidth,		heightMap[x+(z+1)*m_nWidth],	(z+1)*m_nCellWidth);

			glTexCoord2f((float)(x+1)/(float)m_nWidth,(float)z/(float)m_nWidth);
			glVertex3i((x+1)*m_nCellWidth,	heightMap[x+1+z*m_nWidth],	z*m_nCellWidth);

			glTexCoord2f((float)(x+1)/(float)m_nWidth,(float)(z+1)/(float)m_nWidth);
			glVertex3i((x+1)*m_nCellWidth,	heightMap[x+1+(z+1)*m_nWidth],(z+1)*m_nCellWidth);
		}
		glEnd();
		
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		if(escapeTime >= .2f)
		{
			alpha += offset;
			escapeTime = 0.0f;
			if(alpha >= 0.66f)
				offset = -0.08f;
			if(alpha <= 0.1f)
				offset = 0.08f;
		}
		glColor4f(1.f,0.f,0.f,alpha);
		glBegin(GL_QUAD_STRIP);
		for(x=(int)(camera_pos.x/m_nCellWidth-scanDepth),x=x<0?0:x;(x<camera_pos.x/m_nCellWidth+scanDepth)&&x<m_nWidth;++x)
		{
			if((obstructx1 == x && z > obstructz1 && z < obstructz2))
			{
				glVertex3i((x + 1) * m_nCellWidth - 8.0f, heightMap[(x + 1) + z * m_nWidth], z*m_nCellWidth);
				glVertex3i((x + 1) * m_nCellWidth - 8.0f, heightMap[(x + 1) + z * m_nWidth] + stock_height, z*m_nCellWidth);
				glVertex3i((x + 1) * m_nCellWidth - 8.0f, heightMap[(x + 1) + (z + 1) * m_nWidth]  + stock_height, (z + 1)*m_nCellWidth);
				glVertex3i((x + 1) * m_nCellWidth - 8.0f, heightMap[(x + 1) + (z + 1) * m_nWidth], (z + 1) * m_nCellWidth);
			}
			if((obstructx2 == x && z > obstructz1 && z < obstructz2))
			{
				glVertex3i(x * m_nCellWidth + 8.0f, heightMap[(x + 1) + z * m_nWidth], z*m_nCellWidth);
				glVertex3i(x * m_nCellWidth + 8.0f, heightMap[(x + 1) + z * m_nWidth] + stock_height, z*m_nCellWidth);
				glVertex3i(x * m_nCellWidth + 8.0f, heightMap[(x + 1) + (z + 1) * m_nWidth]  + stock_height, (z + 1)*m_nCellWidth);
				glVertex3i(x * m_nCellWidth + 8.0f, heightMap[(x + 1) + (z + 1) * m_nWidth], (z + 1) * m_nCellWidth);
			}
			if(obstructz1 == z && x >=obstructx1 && x <= obstructx2)
			{
				glVertex3i(x * m_nCellWidth, heightMap[x + z * m_nWidth], (z + 1)*m_nCellWidth - 8.0f);
				glVertex3i(x * m_nCellWidth, heightMap[x + z * m_nWidth] + 10.0f, (z + 1)*m_nCellWidth - 8.0f);
				glVertex3i((x + 1) * m_nCellWidth, heightMap[x + 1 + z * m_nWidth]  + stock_height, (z + 1)*m_nCellWidth - 8.0f);
				glVertex3i((x + 1) * m_nCellWidth, heightMap[x + 1 + z * m_nWidth], (z + 1) * m_nCellWidth - 8.0f);
			}
			if(obstructz2 == z && x >=obstructx1 && x <= obstructx2)
			{
				glVertex3i(x * m_nCellWidth, heightMap[x + z * m_nWidth], z * m_nCellWidth + 8.0f);
				glVertex3i(x * m_nCellWidth, heightMap[x + z * m_nWidth] + stock_height, z * m_nCellWidth + 8.0f);
				glVertex3i((x + 1) * m_nCellWidth, heightMap[x + 1 + z * m_nWidth]  + stock_height, z * m_nCellWidth + 8.0f);
				glVertex3i((x + 1) * m_nCellWidth, heightMap[x + 1 + z * m_nWidth], z * m_nCellWidth + 8.0f);
			}
		}
		glEnd();
		glDisable(GL_BLEND);
		glColor4f(1.f,1.f,1.f,1.0f);
	}


}
