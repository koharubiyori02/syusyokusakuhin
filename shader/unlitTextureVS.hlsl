
#include "common.hlsl"

StructuredBuffer<float3> Position : register(t2);

void main(in VS_IN In, out PS_IN Out)
{

    Out.Position = mul(In.Position, World);
    //In.Position.x += In.InstanceId * 2.0f;
    Out.Position.xyz += Position[In.InstansID];
	
    Out.Position = mul(Out.Position, View);
    Out.Position = mul(Out.Position, Projection);
	
    Out.TexCoord = In.TexCoord;
    Out.Diffuse = In.Diffuse * Material.Diffuse;
}

