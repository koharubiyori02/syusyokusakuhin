
#include "common.hlsl"


Texture2D		g_Texture : register(t0);
SamplerState	g_SamplerState : register(s0);


void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float4 textureColor = g_Texture.Sample(g_SamplerState, In.TexCoord);
  
    // UV座標の外枠エフェクト計算
    float edgeX = min(In.TexCoord.x, 1.0 - In.TexCoord.x); // 左右のエッジ距離
    float edgeY = min(In.TexCoord.y, 1.0 - In.TexCoord.y); // 上下のエッジ距離
    float edgeDist = min(edgeX, edgeY - 0.015f); // 最も近いエッジまでの距離
    // エッジ部分の発光判定 (外枠部分の幅を0.1に設定)
    float glow = smoothstep(0.03, 0.0, edgeDist); // 外枠の幅を設定

    // 発光色 (好きな色に変更可能)
    //float4 glowColor = float4(1.0, 0.8, 0.2, 1.0) * glow; // 明るい黄色
    float4 glowColor = float4(0.3, 0.0, 1.0, 1.0) * glow; // 明るい黄色

    // 加算ブレンディングで発光を追加
    outDiffuse = textureColor + glowColor;
}
