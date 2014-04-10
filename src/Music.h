/**
 *作者：范修盛
 *2011年10月30日
 */
#pragma once
#ifndef _MUSIC_H_
#define _MUSIC_H_

#include "StdAfx.h"
#include <dmusicc.h>
#include <dmusici.h>


/**\brief
 * DMusic　音乐播放类
 */
class EGMusic
{
public:
	EGMusic();
	~EGMusic();

	static IDirectMusicSegment* LoadMIDISegment(IDirectMusicLoader* pLoader, WCHAR *wszMidiFileName );

	/**\brief
	 * 初始化DMUSIC
	 */
	static BOOL InitMusic();

	/**\brief
	 * 载入MIDI音乐文件
	 */
	BOOL LoadMusic(WCHAR *);
	BOOL LoadMusic(char* filename){return LoadMusic(WChar(filename));}

	/**\brief
	 * 开始播放
	 */
	BOOL Play(int start=0);

	/**\brief
	 * 停止播放
	 */
	BOOL Stop();

	/**\brief
	 * 暂停播放
	 */
	BOOL Pause();

	/**\brief
	 * 暂停后用来继续播放
	 */
	BOOL Resume();

	/**\brief
	 * 是否正在播放
	 */
	BOOL IsPlay();

	/**\brief
	 * 得到当前音乐对象的指针
	 */
	static EGMusic* GetSysMusic(){return m_pMusic;}

	void	SetMusic(BOOL music){bMusic = music;};
	BOOL	GetMusic(){return bMusic;};

private:
	//是否有音乐
	static BOOL bMusic;

	//主缓冲是否已存在
	static BOOL bExist;
	
	//是否播放
	static BOOL bPlay;
	static IDirectMusicPerformance* g_pPerf;
	static IDirectMusicLoader* g_pLoader;
	IDirectMusicSegment* g_pMIDISeg;

	//暂停点
	MUSIC_TIME PauseTime;

	//文件
	WCHAR MusicFile[MAX_PATH];

	// 对象指针
	static EGMusic* m_pMusic;

	static IDirectMusicPerformance* CreatePerformance(void);
	static IDirectMusicLoader* CreateLoader(void);
};

#endif
