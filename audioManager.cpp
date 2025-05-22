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

    //BGM�����[�h
    LoadAudio(m_BgmPool, AudioName::BGM_Title, "asset\\audio\\BGM\\titleBGM.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_StageSelect, "asset\\audio\\BGM\\stageSelect.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_Tutorial, "asset\\audio\\BGM\\tutorial.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_Game, "asset\\audio\\BGM\\game.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_GameClear, "asset\\audio\\BGM\\gameClear.wav");
    LoadAudio(m_BgmPool, AudioName::BGM_GameOver, "asset\\audio\\BGM\\gameOver.wav");

    //SE�����[�h
    LoadAudio(m_SePool, AudioName::SE_Bullet, "asset\\audio\\SE\\shoot.wav");
    LoadAudio(m_SePool, AudioName::SE_BulletHit, "asset\\audio\\SE\\hit.wav");
    LoadAudio(m_SePool, AudioName::SE_Punch, "asset\\audio\\SE\\punch.wav");
    LoadAudio(m_SePool, AudioName::SE_Tatakituke, "asset\\audio\\SE\\tatakituke.wav");
    LoadAudio(m_SePool, AudioName::SE_Roar, "asset\\audio\\SE\\roar.wav");
    LoadAudio(m_SePool, AudioName::SE_Item, "asset\\audio\\SE\\����{�^��������37.wav");

    //���ʂ�S�Č��炷
    for (int i = 1; i < static_cast<int>(AudioName::AUDIO_NUM); i++)
    {
        SetVolume(static_cast<AudioName>(i), 0.3f);
    }
}

void AudioManager::Uninit()
{
    //BGM�̉��
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

    //SE�̉��
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

    //���
    if (m_MasteringVoice) m_MasteringVoice->DestroyVoice();
    if (m_Xaudio) m_Xaudio->Release();

    CoUninitialize();
}

void AudioManager::LoadAudio(std::unordered_map<AudioName, AudioData>& pool, const AudioName& name, const char* fileName)
{
    WAVEFORMATEX wfx = { 0 };
    BYTE* soundData = nullptr;
    int length = 0, playLength = 0;

    //�����t�@�C���̃��[�h
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

    //�����쐬
    AudioData data = {};
    data.soundData = soundData;
    data.length = length;
    data.playLength = playLength;

    for (int i = 0; i < 100; ++i)//MAX��
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

    //�g�p�\�ȉ���T��
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

    // ���łɍĐ����̃{�C�X�����邩�`�F�b�N
    for (auto* voice : data.sourceVoices)
    {
        XAUDIO2_VOICE_STATE state;
        voice->GetState(&state);
        if (state.BuffersQueued > 0)
        {
            //�Đ����Ȃ�Đ������ɏI��
            return;
        }
    }

    // �Đ����łȂ���΍Đ�
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

    // �Đ����ł���Έꎞ��~
    if (state.BuffersQueued > 0)
    {
        voice->Stop();
        cc[name] = true;//�ꎞ��~��Ԃ��L�^
    }
}

void AudioManager::Resume(const AudioName& name)
{
    if (m_BgmPool.find(name) == m_BgmPool.end()) return;

    if (cc[name])
    {
        auto& data = m_BgmPool[name];
        auto* voice = data.sourceVoices[0];//ONLYONE

        voice->Start(); // �Đ��ĊJ
        cc[name] = false; // �ꎞ��~����
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
    // �{�����[���͈͂̐��� (0.0f = ����, 1.0f = �ő剹��)
    volume = max(0.0f, min(volume, 1.0f));

    // BGM�̉��ʂ�ݒ�
    if (m_BgmPool.find(name) != m_BgmPool.end())
    {
        auto& data = m_BgmPool[name];
        if (!data.sourceVoices.empty())
        {
            data.sourceVoices[0]->SetVolume(volume); // BGM��1�̃{�C�X�̂�
        }
        return;
    }

    // SE�̉��ʂ�ݒ�
    if (m_SePool.find(name) != m_SePool.end())
    {
        auto& data = m_SePool[name];
        for (auto* voice : data.sourceVoices) // SE�͕����̃{�C�X���Đ��\
        {
            voice->SetVolume(volume);
        }
    }
}


void AudioManager::SetMasterVolume(float volume)
{
    // ���ʔ͈͂̐��� (0.0f = ����, 1.0f = �ő剹��)
    volume = max(0.0f, min(volume, 1.0f));

    // BGM�S�̂̉��ʂ�ݒ�
    for (auto& [name, data] : m_BgmPool)
    {
        if (!data.sourceVoices.empty())
        {
            data.sourceVoices[0]->SetVolume(volume); // BGM��1�̃{�C�X�̂�
        }
    }

    // SE�S�̂̉��ʂ�ݒ�
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

    // ���ׂĂ�3D�{�C�X���~
    for (auto* voice : data.sourceVoices)
    {
        voice->Stop();
        voice->FlushSourceBuffers();
    }

    // �N���A(�s�s���Ȃ�����Ă�������)
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

    if (loop) //���[�v
    {
        buffer.LoopBegin = 0;
        buffer.LoopLength = data.playLength;
        buffer.LoopCount = XAUDIO2_LOOP_INFINITE;
    }

    voice->SubmitSourceBuffer(&buffer);
    voice->Start();
    //�o�^
    m_Audio3DPool[name] = { position, maxDistance, true };
}

void AudioManager::UpdatePlay3D(const AudioName& name, const XMFLOAT3& targetPosition)
{
    if (m_Audio3DPool.find(name) == m_Audio3DPool.end() || !m_Audio3DPool[name].isPlaying) return;

    auto& audio3D = m_Audio3DPool[name];
    auto& data = m_SePool[name];
    audio3D.targetPosition = targetPosition;

    //�����v�Z
    float distance = XMVectorGetX(XMVector3Length(XMLoadFloat3(&audio3D.targetPosition)));

    //���ʌv�Z
    float volume = 1.0f - (distance / audio3D.maxDistance);
    volume = max(volume, 0.0f); //����-0�ȉ��ɂȂ�Ȃ�

    auto* voice = data.sourceVoices[0];
    voice->SetVolume(volume);
}
