#include "audioManager.h"

IXAudio2* AudioManager::m_Xaudio = nullptr;
IXAudio2MasteringVoice* AudioManager::m_MasteringVoice = nullptr;
std::unordered_map<AudioName, AudioManager::AudioData> AudioManager::m_BgmPool{};
std::unordered_map<AudioName, AudioManager::AudioData> AudioManager::m_SePool{};
std::unordered_map<AudioName, bool> AudioManager::cc{};
std::unordered_map<AudioName, AudioManager::Audio3DData> AudioManager::m_Audio3DPool{};


void AudioManager::Init()
{
    CoInitializeEx(NULL, COINIT_MULTITHREADED);
    XAudio2Create(&m_Xaudio, 0);
    m_Xaudio->CreateMasteringVoice(&m_MasteringVoice);

    //BGMをロード
    LoadAudio(m_BgmPool, AudioName::BGM_Title, "asset\\audio\\BGM\\titleBGM.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_StageSelect, "asset\\audio\\BGM\\stageSelect.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_Tutorial, "asset\\audio\\BGM\\tutorial.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_Game, "asset\\audio\\BGM\\game.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_GameClear, "asset\\audio\\BGM\\gameClear.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_GameOver, "asset\\audio\\BGM\\gameOver.wav");

    //SEをロード
    LoadAudio(m_SePool, AudioName::SE_Bullet, "asset\\audio\\SE\\shoot.wav");
    LoadAudio(m_SePool, AudioName::SE_BulletHit, "asset\\audio\\SE\\hit.wav");
    LoadAudio(m_SePool, AudioName::SE_Punch, "asset\\audio\\SE\\punch.wav");
    LoadAudio(m_SePool, AudioName::SE_Tatakituke, "asset\\audio\\SE\\tatakituke.wav");
    LoadAudio(m_SePool, AudioName::SE_Roar, "asset\\audio\\SE\\roar.wav");
    LoadAudio(m_SePool, AudioName::SE_Item, "asset\\audio\\SE\\決定ボタンを押す37.wav");

    //音量を全て減らす
    for (int i = 1; i < static_cast<int>(AudioName::AUDIO_NUM); i++)
    {
        SetVolume(static_cast<AudioName>(i), 0.3f);
    }
}

void AudioManager::Uninit()
{
    //BGMの解放
    for (auto& [name, data] : m_BgmPool)
    {
        for (auto* voice : data.sourceVoices)
        {
            voice->Stop();
            voice->DestroyVoice();
        }
        delete[] data.soundData;
    }
    m_BgmPool.clear();

    //SEの解放
    for (auto& [name, data] : m_SePool)
    {
        for (auto* voice : data.sourceVoices)
        {
            voice->Stop();
            voice->DestroyVoice();
        }
        delete[] data.soundData;
    }
    m_SePool.clear();

    //解放
    if (m_MasteringVoice) m_MasteringVoice->DestroyVoice();
    if (m_Xaudio) m_Xaudio->Release();

    CoUninitialize();
}

void AudioManager::LoadAudio(std::unordered_map<AudioName, AudioData>& pool, const AudioName& name, const char* fileName)
{
    WAVEFORMATEX wfx = { 0 };
    BYTE* soundData = nullptr;
    int length = 0, playLength = 0;

    //音声ファイルのロード
    {
        HMMIO hmmio = mmioOpen((LPSTR)fileName, nullptr, MMIO_READ);
        assert(hmmio);

        MMCKINFO riffChunk = { 0 };
        MMCKINFO fmtChunk = { 0 };
        MMCKINFO dataChunk = { 0 };
        mmioDescend(hmmio, &riffChunk, nullptr, MMIO_FINDRIFF);

        fmtChunk.ckid = mmioFOURCC('f', 'm', 't', ' ');
        mmioDescend(hmmio, &fmtChunk, &riffChunk, MMIO_FINDCHUNK);
        mmioRead(hmmio, (HPSTR)&wfx, sizeof(wfx));
        mmioAscend(hmmio, &fmtChunk, 0);

        dataChunk.ckid = mmioFOURCC('d', 'a', 't', 'a');
        mmioDescend(hmmio, &dataChunk, &riffChunk, MMIO_FINDCHUNK);

        length = dataChunk.cksize;
        soundData = new BYTE[length];
        mmioRead(hmmio, (HPSTR)soundData, length);
        playLength = length / wfx.nBlockAlign;

        mmioClose(hmmio, 0);
    }

    //複数作成
    AudioData data = {};
    data.soundData = soundData;
    data.length = length;
    data.playLength = playLength;

    for (int i = 0; i < 100; ++i)//MAX数
    {
        IXAudio2SourceVoice* sourceVoice = nullptr;
        m_Xaudio->CreateSourceVoice(&sourceVoice, &wfx);
        assert(sourceVoice);
        data.sourceVoices.push_back(sourceVoice);
    }

    pool[name] = data;
}

void AudioManager::Play(const AudioName& name, bool loop)
{
    if (m_BgmPool.find(name) == m_BgmPool.end()) return;

    auto& data = m_BgmPool[name];
    auto* voice = data.sourceVoices[0];//onlyOne
    voice->Stop();
    voice->FlushSourceBuffers();

    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = data.length;
    buffer.pAudioData = data.soundData;
    buffer.PlayBegin = 0;
    buffer.PlayLength = data.playLength;

    if (loop)
    {
        buffer.LoopBegin = 0;
        buffer.LoopLength = data.playLength;
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    voice->SubmitSourceBuffer(&buffer);
    voice->Start();
}

void AudioManager::PlaySE(const AudioName& name)
{
    if (m_SePool.find(name) == m_SePool.end()) return;

    auto& data = m_SePool[name];

    //使用可能な音を探す
    for (auto* voice : data.sourceVoices)
    {
        XAUDIO2_VOICE_STATE state;
        voice->GetState(&state);
        if (state.BuffersQueued == 0)
        {
            XAUDIO2_BUFFER buffer = {};
            buffer.AudioBytes = data.length;
            buffer.pAudioData = data.soundData;
            voice->SubmitSourceBuffer(&buffer);
            voice->Start();
            return;
        }
    }
}

void AudioManager::PlaySENotPlaying(const AudioName& name)
{
    if (m_SePool.find(name) == m_SePool.end()) return;

    auto& data = m_SePool[name];

    // すでに再生中のボイスがあるかチェック
    for (auto* voice : data.sourceVoices)
    {
        XAUDIO2_VOICE_STATE state;
        voice->GetState(&state);
        if (state.BuffersQueued > 0)
        {
            //再生中なら再生せずに終了
            return;
        }
    }

    // 再生中でなければ再生
    for (auto* voice : data.sourceVoices)
    {
        XAUDIO2_VOICE_STATE state;
        voice->GetState(&state);
        if (state.BuffersQueued == 0)
        {
            XAUDIO2_BUFFER buffer = {};
            buffer.AudioBytes = data.length;
            buffer.pAudioData = data.soundData;
            voice->SubmitSourceBuffer(&buffer);
            voice->Start();
            break;
        }
    }
}

void AudioManager::Stop(const AudioName& name)
{
    if (m_BgmPool.find(name) != m_BgmPool.end())
    {
        for (auto* voice : m_BgmPool[name].sourceVoices)
        {
            voice->Stop();
        }
    }
    if (m_SePool.find(name) != m_SePool.end())
    {
        for (auto* voice : m_SePool[name].sourceVoices)
        {
            voice->Stop();
        }
    }
}

void AudioManager::Pause(const AudioName& name)
{
    if (m_BgmPool.find(name) == m_BgmPool.end()) return;

    auto& data = m_BgmPool[name];
    auto* voice = data.sourceVoices[0];//OnlyONE
    XAUDIO2_VOICE_STATE state;
    voice->GetState(&state);

    // 再生中であれば一時停止
    if (state.BuffersQueued > 0)
    {
        voice->Stop();
        cc[name] = true;//一時停止状態を記録
    }
}

void AudioManager::Resume(const AudioName& name)
{
    if (m_BgmPool.find(name) == m_BgmPool.end()) return;

    if (cc[name])
    {
        auto& data = m_BgmPool[name];
        auto* voice = data.sourceVoices[0];//ONLYONE

        voice->Start(); // 再生再開
        cc[name] = false; // 一時停止解除
    }
}




void AudioManager::StopAll()
{
    for (auto& [name, data] : m_BgmPool)
    {
        for (auto* voice : data.sourceVoices)
        {
            voice->Stop();
        }
    }
    for (auto& [name, data] : m_SePool)
    {
        for (auto* voice : data.sourceVoices)
        {
            voice->Stop();
        }
    }
}

void AudioManager::SetVolume(const AudioName& name, float volume)
{
    // ボリューム範囲の制限 (0.0f = 無音, 1.0f = 最大音量)
    volume = max(0.0f, min(volume, 1.0f));

    // BGMの音量を設定
    if (m_BgmPool.find(name) != m_BgmPool.end())
    {
        auto& data = m_BgmPool[name];
        if (!data.sourceVoices.empty())
        {
            data.sourceVoices[0]->SetVolume(volume); // BGMは1つのボイスのみ
        }
        return;
    }

    // SEの音量を設定
    if (m_SePool.find(name) != m_SePool.end())
    {
        auto& data = m_SePool[name];
        for (auto* voice : data.sourceVoices) // SEは複数のボイスを再生可能
        {
            voice->SetVolume(volume);
        }
    }
}


void AudioManager::SetMasterVolume(float volume)
{
    // 音量範囲の制限 (0.0f = 無音, 1.0f = 最大音量)
    volume = max(0.0f, min(volume, 1.0f));

    // BGM全体の音量を設定
    for (auto& [name, data] : m_BgmPool)
    {
        if (!data.sourceVoices.empty())
        {
            data.sourceVoices[0]->SetVolume(volume); // BGMは1つのボイスのみ
        }
    }

    // SE全体の音量を設定
    for (auto& [name, data] : m_SePool)
    {
        for (auto* voice : data.sourceVoices)
        {
            voice->SetVolume(volume);
        }
    }
}

void AudioManager::StopPlay3D(const AudioName& name)
{
    if (m_SePool.find(name) == m_SePool.end()) return;

    auto& data = m_SePool[name];

    // すべての3Dボイスを停止
    for (auto* voice : data.sourceVoices)
    {
        voice->Stop();
        voice->FlushSourceBuffers();
    }

    // クリア(不都合なら消してください)
    //data.sourceVoices.clear();
}

void AudioManager::Play3D(const AudioName & name, const XMFLOAT3 & position, float maxDistance, bool loop)
{
    if (m_SePool.find(name) == m_SePool.end()) return;

    auto& data = m_SePool[name];
    auto* voice = data.sourceVoices[0];

    voice->Stop();
    voice->FlushSourceBuffers();

    XAUDIO2_BUFFER buffer = {};
    buffer.AudioBytes = data.length;
    buffer.pAudioData = data.soundData;
    buffer.PlayBegin = 0;
    buffer.PlayLength = data.playLength;

    if (loop) //ループ
    {
        buffer.LoopBegin = 0;
        buffer.LoopLength = data.playLength;
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    voice->SubmitSourceBuffer(&buffer);
    voice->Start();
    //登録
    m_Audio3DPool[name] = { position, maxDistance, true };
}

void AudioManager::UpdatePlay3D(const AudioName& name, const XMFLOAT3& targetPosition)
{
    if (m_Audio3DPool.find(name) == m_Audio3DPool.end() || !m_Audio3DPool[name].isPlaying) return;

    auto& audio3D = m_Audio3DPool[name];
    auto& data = m_SePool[name];
    audio3D.targetPosition = targetPosition;

    //距離計算
    float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&audio3D.targetPosition)));

    //音量計算
    float volume = 1.0f - (distance / audio3D.maxDistance);
    volume = max(volume, 0.0f); //音量-0以下にならない

    auto* voice = data.sourceVoices[0];
    voice->SetVolume(volume);
}
