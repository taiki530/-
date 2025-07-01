#include "common.hlsl"


Texture2D g_Texture : register(t0);
Texture2D g_TextureEnv : register(t1);
SamplerState g_SamplerState : register(s0);
static float PI = 3.141592;

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);
    
    float3 normal = normalize(In.Normal.xyz);
    
    //ã¸ê‹
    float3 refrv = reflect(eyev.xyz, normal);
    refrv = normalize(refrv);

    //ÉpÉmÉâÉ}
    float2 envTexCoord;
    envTexCoord.x = atan2(refrv.x, refrv.z) / (PI * 2);
    envTexCoord.y = acos(refrv.y) / PI;
    
    outDiffuse.rgb = g_TextureEnv.Sample(g_SamplerState, envTexCoord);
    outDiffuse.a = 1.0;
}
