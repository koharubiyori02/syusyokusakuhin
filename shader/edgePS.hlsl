
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 textureColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
  
    // UV���W�̊O�g�G�t�F�N�g�v�Z
    float edgeX = min(In.TexCoord.x, 1.0 - In.TexCoord.x); // ���E�̃G�b�W����
    float edgeY = min(In.TexCoord.y, 1.0 - In.TexCoord.y); // �㉺�̃G�b�W����
    float edgeDist = min(edgeX, edgeY - 0.015f); // �ł��߂��G�b�W�܂ł̋���
    // �G�b�W�����̔������� (�O�g�����̕���0.1�ɐݒ�)
    float glow = smoothstep(0.03, 0.0, edgeDist); // �O�g�̕���ݒ�

    // �����F (�D���ȐF�ɕύX�\)
    //float4 glowColor = float4(1.0, 0.8, 0.2, 1.0) * glow; // ���邢���F
    float4 glowColor = float4(0.3, 0.0, 1.0, 1.0) * glow; // ���邢���F

    // ���Z�u�����f�B���O�Ŕ�����ǉ�
    outDiffuse = textureColor + glowColor;
}
