#include "StdAfx.h"
#include "MP3.h"

#include <uuids.h>

CMP3::CMP3(void)
{
	m_pigb = NULL;
	m_pimc = NULL;
	m_pimex = NULL;
	m_piba = NULL;
	m_pims = NULL;
	m_fReady = FALSE;
	m_iduration = 0;
}

CMP3::~CMP3(void)
{
	Cleanup();
}

VOID CMP3::Cleanup()
{
	if(m_pimc != NULL)
		m_pimc->Stop();
	if (m_pigb != NULL)
	{
		m_pigb->Release();
		m_pigb = NULL;
	}
	if(m_pimc != NULL)
	{
		m_pimc->Release();
		m_pimc = NULL;
	}

	if (m_pimex != NULL)
	{
		m_pimex->Release();
		m_pimex = NULL;
	}

	if (m_piba != NULL)
	{
		m_piba->Release();
		m_piba = NULL;
	}

	if (m_pims != NULL)
	{
		m_pims->Release();
		m_pims = NULL;
	}
	m_fReady = FALSE;
}

BOOL CMP3::Load(LPCWSTR p_lpMP3FilePath)
{
	Cleanup();
	m_fReady = FALSE;

	HRESULT hr = S_OK ;
	hr = CoCreateInstance( CLSID_FilterGraph , NULL , CLSCTX_INPROC_SERVER ,
		IID_IGraphBuilder , (void**)&this->m_pigb);

	if(SUCCEEDED(hr)){
		hr = m_pigb->QueryInterface( IID_IMediaControl ,(void**)&m_pimc);
	}
	if(SUCCEEDED(hr)){
		hr = m_pigb->QueryInterface( IID_IMediaEventEx , (void**)&m_pimex);
	}
	if(SUCCEEDED(hr)){
		hr = m_pigb->QueryInterface(IID_IBasicAudio, (void**)&m_piba);
	}

	if(SUCCEEDED(hr)){
		hr = m_pigb->QueryInterface(IID_IMediaSeeking, (void**)&m_pims);
	}

	if(SUCCEEDED(hr)){
		hr = m_pigb->RenderFile( p_lpMP3FilePath , NULL );
	}

	if(SUCCEEDED(hr)){
		m_fReady = TRUE;
		if(m_pims != NULL)
		{
			m_pims->SetTimeFormat( &TIME_FORMAT_MEDIA_TIME) ;
			m_pims->GetDuration( &m_iduration );// returns 10,000,000 for a second.
		}
	}

	return m_fReady;
}


BOOL CMP3::Play()
{
	if(m_fReady && m_pimc != NULL){
		return (SUCCEEDED(m_pimc->Run()));
	}
	return FALSE;
}

BOOL CMP3::Pause()
{
	if(m_fReady && m_pimc != NULL){
		return (SUCCEEDED(m_pimc->Pause()));
	}
	return FALSE;
}

BOOL CMP3::Stop()
{
	if(m_fReady && m_pimc != NULL){
		return (SUCCEEDED(m_pimc->Stop()));
	}
	return FALSE;
}

BOOL CMP3::WaitForCompletion(long p_msTimeout, long* p_EvCode)
{
	if(m_fReady && m_pimex != NULL){
		return ((SUCCEEDED(m_pimex->WaitForCompletion(p_msTimeout ,p_EvCode)))&&
			(*p_EvCode >0));
	}

	return FALSE;
}

BOOL CMP3::SetVolume(long p_vol)
{
	if(m_fReady && m_piba != NULL){
		return (SUCCEEDED(m_piba->put_Volume(p_vol)));
	}

	return FALSE;
}

long CMP3::GetVolume()
{
	if(m_fReady && m_piba != NULL){
		long vol = -1;
		if(SUCCEEDED(m_piba->get_Volume(&vol))){
			return vol;
		}
	}

	return -1;
}

__int64 CMP3::GetDuration()
{
	return m_iduration;
}

__int64 CMP3::GetCurrentPosition()
{
	if(m_fReady && m_pims != NULL){
		__int64 curPos = -1;
		if(SUCCEEDED (m_pims->GetCurrentPosition(&curPos)))
			return curPos;
	}

	return -1;
}

BOOL CMP3::SetPositions(__int64* p_pCurrent, __int64* p_pStop, BOOL p_fAbsolutePositioning)
{
	if (m_fReady && m_pims != NULL)
	{
		DWORD dwFlags = 0;
		if(p_fAbsolutePositioning)
			dwFlags = AM_SEEKING_AbsolutePositioning| AM_SEEKING_SeekToKeyFrame;
		else
			dwFlags = AM_SEEKING_RelativePositioning | AM_SEEKING_SeekToKeyFrame;

		if(SUCCEEDED(m_pims->SetPositions(p_pCurrent , dwFlags , p_pStop , dwFlags)))
			return TRUE;
	}

	return FALSE;
}