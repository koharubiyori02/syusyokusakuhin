//===========================================================
//
// Audio�}�l�[�W���[
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
        std::vector<IXAudio2SourceVoice*> sourceVoices; //�����̃{�C�X���Ǘ�
        BYTE* soundData = nullptr;
        int length = 0;
        int playLength = 0;
    };
    // 3D�����Ǘ��p
    struct Audio3DData
    {
        XMFLOAT3 targetPosition;
        float maxDistance;
        bool isPlaying = false;
    };

    static std::unordered_map<AudioName, AudioData> m_BgmPool; //BGM�v�[��

    static std::unordered_map<AudioName, AudioData> m_SePool;  //SE�v�[��

    static std::unordered_map<AudioName, bool> cc; //�ꎞ��~��Ԃ̋L�^

    static std::unordered_map<AudioName, Audio3DData> m_Audio3DPool;//3DPlay�pData

    static void LoadAudio(std::unordered_map<AudioName, AudioData>& pool, const AudioName& name, const char* fileName);

public:
    static void Init();

    static void Uninit();

    static void Play(const AudioName& name, bool loop = false);

    static void Stop(const AudioName& name);

    static void PlaySE(const AudioName& name); //SE��p�Đ�

    static void PlaySENotPlaying(const AudioName& name);//����Ȃ��Ă��Ȃ���΍Đ�

    static void Pause(const AudioName& name); // BGM��p��Pause

    static void Resume(const AudioName& name); //�ꎞ��~���������čĐ�

    static void StopAll();//���ׂẲ����~

    static void SetVolume(const AudioName& name, float volume); // ���ʂ�ݒ�

    static void SetMasterVolume(float volume); // �S�̂̉��ʂ�ݒ�

    // 3D�����p�֐�
    static void Play3D(const AudioName& name, const XMFLOAT3 & position, float maxDistance, bool loop = false);

    static void UpdatePlay3D(const AudioName& name, const XMFLOAT3& targetPosition);

    static void StopPlay3D(const AudioName& name);
};