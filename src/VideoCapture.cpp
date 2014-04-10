/**
 *作者：范修盛
 *2011年10月30日
 */
#include "VideoCapture.h"

#define OBJ_MOVE_ID		1
#define OBJ_MOVE		0
#define OBJ_SKILL_ID	2
#define OBJ_SKILL		1


/** 单帧最大扫描次数**/
static int thresh;
/** 从当前摄像头获取的一帧图片 **/
static ARUint8 * m_pImgCaptureFromeCamera;
/** 从当前摄像头获取的一帧图片 **/
//static ARInt16 * m_pImgCaptureFromeCamera16;
/** 目标卡片对象 **/
static OBJECT_T object[2];
/** 摄像头的参数 **/
static ARParam         cparam, wparam;
/** 摄像头窗口大小 **/
static int	xsize, ysize;
/** 临界同步锁 **/
//static CCriticalSection  m_cCriticalSection ;
/** AR运行当中 **/
static BOOL isRunning;
static BOOL isHeightSpeedCamera;

VideoCapture::VideoCapture(void)
{
}

VideoCapture::~VideoCapture(void)
{}

/** 检测运行状态 **/
BOOL VideoCapture::getRunState(){return isRunning;};


void VideoCapture::UpThresh()
{
	thresh+=10;
}

void VideoCapture::DownThresh()
{
	thresh-=10;
}

int VideoCapture::GetThresh()
{
	return thresh;
}

/**\brief
 *初始化摄像头
 */
BOOL VideoCapture::LoadIni(char* _filename,char* _index)
{
	/** 摄像头配置文件 **/
	char * vconf, * cparam_name;

	EGCIniFile Ini(_filename);

	vconf = Ini.ReadText(_index,"vconf");
	cparam_name = Ini.ReadText(_index,"cparam_name");
	thresh = Ini.ReadInt(_index,"thresh");

	object[OBJ_MOVE].patt_name = Ini.ReadText(_index,"patt_move");
	object[OBJ_MOVE].patt_id = -1;
	object[OBJ_MOVE].visible = 0;
	object[OBJ_MOVE].model_id = OBJ_MOVE_ID;
	object[OBJ_MOVE].width = Ini.ReadInt(_index,"move_size");
	object[OBJ_MOVE].center[0] = 0.0;
	object[OBJ_MOVE].center[1] = 0.0;

	object[OBJ_SKILL].patt_name = Ini.ReadText(_index,"patt_skill");
	object[OBJ_SKILL].patt_id = -1;
	object[OBJ_SKILL].visible = 0;
	object[OBJ_SKILL].model_id = OBJ_SKILL_ID;
	object[OBJ_SKILL].width = Ini.ReadInt(_index,"skill_size");
	object[OBJ_SKILL].center[0] = 0.0;
	object[OBJ_SKILL].center[1] = 0.0;

	isRunning = FALSE;
	isHeightSpeedCamera = TRUE;
	/** 设置文件 */
    if( arVideoOpen( vconf ) < 0)
	{
		ShowMessage("找不到摄像头配置文件！");
        return FALSE;
	}
    
	/* 找出视频窗口的大小 */
    if( arVideoInqSize(&xsize, &ysize) < 0 )
	{
		ShowMessage("无法获取摄像头图像的大小！");
        return FALSE;
	}

    /* 设置摄像头初始化参数 */
    if( arParamLoad(cparam_name, 1, &wparam) < 0 ) {
        printf("Camera parameter load error !!\n");
        return FALSE;
    }

    arParamChangeSize( &wparam, xsize, ysize, &cparam );
    arInitCparam( &cparam );

	/** 装载匹配模型数据 **/
    for(int i = 0; i < 2; i++ ) {
        if( (object[i].patt_id=arLoadPatt(object[i].patt_name)) < 0 ) {
			char * str = new char [126];
            sprintf(str,"pattern load error: %s\n", object[i].patt_name);
			ShowMessage(str);
            return FALSE;
        }
    }

	return TRUE;
}

void VideoCapture::run()
{
	isRunning = TRUE;
    /* 打开摄像头监控 */
    argInit( &cparam, 0, 0, 0, 0, 0 );
	/** 重设Timer **/
	arUtilTimerReset();
    arVideoCapStart();
    while(1)
	{
		mainLoop();
	}
}

/**\brief
 *从摄像头获取一帧图像
 */
ARUint8 * VideoCapture::getImgFromCap()
{
	return m_pImgCaptureFromeCamera;
}


/**\brief
 *清理资源
 */
void VideoCapture::cleanup(void)
{
	isRunning = FALSE;
    arVideoCapStop();
    arVideoClose();
    argCleanup();
}

/**\brief
 *获取视频文件的宽和高
 */
int VideoCapture::getWidth()
{
	return xsize;
}
int VideoCapture::getHeight()
{
	return ysize;
}


void VideoCapture::mainLoop(void)
{
    ARMarkerInfo    *marker_info;
    int             marker_num;
    int             i, j, k;
	int lable_num;
	int *area,*clip, *label_ref;
	double *pos;

    /* 尝试获取一帧图像数据 */
    if( (m_pImgCaptureFromeCamera = (ARUint8 *)arVideoGetImage()) == NULL ) {
        Sleep(1);
        return;
    }
	
	/** 检测卡片位置坐标 **/
    if( arDetectMarker(m_pImgCaptureFromeCamera, thresh, &marker_info, &marker_num) < 0 ) {
        exit(0);
    }

    /* 如果检测到卡片信息，就更新卡片结构体的信息 */
    for( i = 0; i < 2; i++ ) {
        k = -1;
        for( j = 0; j < marker_num; j++ ) {
            if( object[i].patt_id == marker_info[j].id ) {
                if( k == -1 ) k = j;
                else if( marker_info[k].cf < marker_info[j].cf ) k = j;
            }
        }
        object[i].visible = k;

        if( k >= 0 ) {
            arGetTransMat(&marker_info[k],
                          object[i].center, object[i].width,
                          object[i].trans);
        }
    }

	/** 高速摄像头每秒15帧 **/
	if(isHeightSpeedCamera)
		Sleep(66);
	else
	/** 为兼容低速摄像头每秒8帧 **/
		Sleep(118);
    arVideoCapNext();
    argSwapBuffers();
}

/** 低帧速摄像头 **/
void VideoCapture::setLowSpeedCamera()
{
	isHeightSpeedCamera = FALSE;
}



/** 获取当前卡片信息结构 **/
OBJECT_T * VideoCapture::getObjects()
{
	return object;
}