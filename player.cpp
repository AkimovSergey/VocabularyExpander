#include "player.h"

Player::Player()
{

}
#define SAMPLE_RATE 44100


Player::~Player()
{
    //if(m_p_thread_record) m_p_thread_record->join();
}

bool Player::PlayMedia(QString path)
{
#if defined(WIN32) && !defined(UNIX)

  /*  IGraphBuilder *pGraph = NULL;
    IMediaControl *pControl = NULL;
    IMediaEvent   *pEvent = NULL;

    // Initialize the COM library.
    HRESULT hr = CoInitialize(NULL);
    if (FAILED(hr)) return false;

    // Create the filter graph manager and query for interfaces.
    hr = CoCreateInstance(CLSID_FilterGraph, NULL, CLSCTX_INPROC_SERVER,
                        IID_IGraphBuilder, (void **)&pGraph);
    if (FAILED(hr))return false;

    hr = pGraph->QueryInterface(IID_IMediaControl, (void **)&pControl);
    hr = pGraph->QueryInterface(IID_IMediaEvent, (void **)&pEvent);

    // Build the graph. IMPORTANT: Change this string to a file on your system.
    hr = pGraph->RenderFile(path.wc_str(), NULL);
    if (SUCCEEDED(hr))
    {
        // Run the graph.
        hr = pControl->Run();
        if (SUCCEEDED(hr))
        {
            // Wait for completion.
            long evCode;
            pEvent->WaitForCompletion(10000, &evCode);

            // Note: Do not use INFINITE in a real application, because it
            // can block indefinitely.
        }
    }
    pControl->Release();
    pEvent->Release();
    pGraph->Release();
    CoUninitialize();*/
#endif

    return true;
}
bool Player::SwitchRecord(bool start_stop)
{
    /*m_b_stop = !start_stop;
    if(!m_b_stop)
    {
        m_condvar_record.notify_all();
        if(m_p_thread_record) m_p_thread_record->join();
        m_p_thread_record.reset( new std::thread(std::bind(&Player::ThreadRecordAndPlay, this)));
    }
    return m_b_stop;*/
    return true;
}

#if defined(WIN32) && !defined(UNIX)

/*VOID CALLBACK Player::waveOutProc(HWAVEOUT hWaveOut, UINT nMessage, DWORD_PTR nInstance, DWORD_PTR nParameter1, DWORD_PTR nParameter2)
{
    Player * pPlayer = (Player*)nInstance;
    if(nMessage == MM_WOM_DONE)
        pPlayer->m_condvar_record.notify_all();

}

VOID CALLBACK Player::waveInProc(HWAVEIN hWaveIn, UINT nMessage, DWORD_PTR nInstance, DWORD_PTR nParameter1, DWORD_PTR nParameter2)
{
    Player * pPlayer = (Player*)nInstance;
    if(pPlayer->m_b_stop)
    {
        pPlayer->m_condvar_record.notify_all();
        return;
    }

    if(nMessage ==  MM_WIM_DATA)
    {
        WAVEHDR * pInHdr = 0;
        pInHdr = new WAVEHDR();
        pPlayer->m_wave_headers.push_back(pInHdr);
        pInHdr->lpData = (LPSTR)new char[SAMPLE_RATE * 2 * 2];
        pInHdr->dwBufferLength = SAMPLE_RATE * 2 * 2;
        pInHdr->dwLoops = 1L;

        if(waveInPrepareHeader(hWaveIn, pInHdr, sizeof(WAVEHDR))
        || waveInAddBuffer(hWaveIn, pInHdr, sizeof(WAVEHDR))
        || waveInStart(hWaveIn))
        {
            Globals::DisplayError(L" Can't record sound ");
            return;
        }
    }

}
//#endif
//void Player::ThreadRecordAndPlay()
//{

#if defined(WIN32) && !defined(UNIX)

    int sampleRate = SAMPLE_RATE;

    WAVEFORMATEX pFormat;
    pFormat.wFormatTag = WAVE_FORMAT_PCM;     // simple, uncompressed format
    pFormat.nChannels = 1;                    //  1=mono, 2=stereo
    pFormat.nSamplesPerSec = sampleRate;      // 44100
    pFormat.nAvgBytesPerSec = sampleRate*2;   // = nSamplesPerSec * n.Channels * wBitsPerSample/8
    pFormat.nBlockAlign = 2;                  // = n.Channels * wBitsPerSample/8
    pFormat.wBitsPerSample = 16;              //  16 for high quality, 8 for telephone-grade
    pFormat.cbSize = 0;

    if( waveInOpen(&m_hWaveIn, WAVE_MAPPER, &pFormat, (DWORD_PTR)waveInProc, (DWORD_PTR)this,
        WAVE_FORMAT_DIRECT | CALLBACK_FUNCTION ) )
    {
        WAVERR_BADFORMAT;
        Globals::DisplayError(L" Microphone not found ");
        return;
    }

    WAVEHDR * pInHdr = 0;
    pInHdr = new WAVEHDR();
    m_wave_headers.push_back(pInHdr);
    pInHdr->lpData = (LPSTR)new char[SAMPLE_RATE * 2 * 2];
    pInHdr->dwBufferLength = SAMPLE_RATE * 2 * 2;
    pInHdr->dwLoops = 1L;

    if(waveInPrepareHeader(m_hWaveIn, pInHdr, sizeof(WAVEHDR))
        || waveInAddBuffer(m_hWaveIn, pInHdr, sizeof(WAVEHDR))
        || waveInStart(m_hWaveIn))
        {
            Globals::DisplayError(L" Can't record sound ");
            return;
        }

    std::unique_lock<std::mutex> lock(m_syncro_record);
    auto res = m_condvar_record.wait_for(lock, std::chrono::seconds(60));
    waveInStop(m_hWaveIn);
    waveInClose(m_hWaveIn);
    if(res == std::cv_status::no_timeout)
    {

        waveOutOpen(&m_hWaveOut, WAVE_MAPPER, &pFormat, (DWORD_PTR)waveOutProc, (DWORD_PTR)this,
            CALLBACK_FUNCTION);

        while(m_wave_headers.size() > 0)
        {
            // while not record again
            if(m_b_stop)
            {

                waveOutPrepareHeader(m_hWaveOut, m_wave_headers.front(), sizeof(WAVEHDR));
                waveOutWrite(m_hWaveOut, m_wave_headers.front(), sizeof(WAVEHDR));
                // just to avoid blocking
                auto res = m_condvar_record.wait_for(lock, std::chrono::seconds(10));
                if(res != std::cv_status::no_timeout)
                    break;
            }

            delete[] m_wave_headers.front()->lpData;
            delete m_wave_headers.front();
            m_wave_headers.pop_front();
        }

        waveOutClose(m_hWaveOut);

    }
#endif

}*/
#endif

