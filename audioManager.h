//===========================================================
//
// Audioマネージャー
// 
//===========================================================

#pragma once

#include <xaudio2.h>
#include <unordered_map>
#include <vector>
#include <string>
#include <cassert>
#include "main.h"

enum class AudioName : unsigned int
{
    ERROR_TYPE_AUDIO_NOT_SET,
    BGM_Title,
    BGM_StageSelect,
    BGM_Tutorial,
    BGM_Game,
    BGM_GameClear,
    BGM_GameOver,
    SE_Bullet,
    SE_BulletHit,
    SE_Punch,
    SE_Tatakituke,
    SE_Roar,
    SE_Item,
    AUDIO_NUM
};

class AudioManager
{
private:
    static IXAudio2* m_Xaudio;

    static IXAudio2MasteringVoice* m_MasteringVoice;

    struct AudioData
    {
        std::vector<IXAudio2SourceVoice*> sourceVoices; //複数のボイスを管理
        BYTE* soundData = nullptr;
        int length = 0;
        int playLength = 0;
    };
    // 3D音声管理用
    struct Audio3DData
    {
        XMFLOAT3 targetPosition;
        float maxDistance;
        bool isPlaying = false;
    };

    static std::unordered_map<AudioName, AudioData> m_BgmPool; //BGMプール

    static std::unordered_map<AudioName, AudioData> m_SePool;  //SEプール

    static std::unordered_map<AudioName, bool> cc; //一時停止状態の記録

    static std::unordered_map<AudioName, Audio3DData> m_Audio3DPool;//3DPlay用Data

    static void LoadAudio(std::unordered_map<AudioName, AudioData>& pool, const AudioName& name, const char* fileName);

public:
    static void Init();

    static void Uninit();

    static void Play(const AudioName& name, bool loop = false);

    static void Stop(const AudioName& name);

    static void PlaySE(const AudioName& name); //SE専用再生

    static void PlaySENotPlaying(const AudioName& name);//一つもなっていなければ再生

    static void Pause(const AudioName& name); // BGM専用のPause

    static void Resume(const AudioName& name); //一時停止を解除して再生

    static void StopAll();//すべての音を停止

    static void SetVolume(const AudioName& name, float volume); // 音量を設定

    static void SetMasterVolume(float volume); // 全体の音量を設定

    // 3D音声用関数
    static void Play3D(const AudioName& name, const XMFLOAT3 & position, float maxDistance, bool loop = false);

    static void UpdatePlay3D(const AudioName& name, const XMFLOAT3& targetPosition);

    static void StopPlay3D(const AudioName& name);
};