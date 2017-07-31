#pragma once

#include <MMSystem.h>
#include <strmif.h>
#pragma comment(lib,"strmiids.lib")
#include <control.h>

class CMP3
{
public:
	CMP3(void);
	virtual ~CMP3(void);

public:
	BOOL Load(LPCWSTR p_lpMP3FilePath);
	VOID Cleanup();
	BOOL Play();
	BOOL Pause();
	BOOL Stop();

	// Poll this function with msTimeout = 0, so that it return immediately.
	// If the mp3 finished playing, WaitForCompletion will return true;
	BOOL WaitForCompletion(long p_msTimeout, long* p_EvCode);

	// -10000 is lowest volume and 0 is highest volume, positive value > 0 will fail
	BOOL SetVolume(long p_vol);

	// -10000 is lowest volume and 0 is highest volume
	long GetVolume();

	// Returns the duration in 1/10 millionth of a second,
	// meaning 10,000,000 == 1 second
	// You have to divide the result by 10,000,000 
	// to get the duration in seconds.
	__int64 GetDuration();

	// Returns the current playing position
	// in 1/10 millionth of a second,
	// meaning 10,000,000 == 1 second
	// You have to divide the result by 10,000,000 
	// to get the duration in seconds.
	__int64 GetCurrentPosition();

	// Seek to position
	BOOL SetPositions(__int64* p_pCurrent, __int64* p_pStop, BOOL p_fAbsolutePositioning);

private:
	IGraphBuilder*	m_pigb;
	IMediaControl*	m_pimc;
	IMediaEventEx*	m_pimex;
	IBasicAudio*	m_piba;
	IMediaSeeking*	m_pims;
	BOOL			m_fReady;
	__int64			m_iduration;
};
