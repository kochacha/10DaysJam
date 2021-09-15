#include "PeraShaderHeader.hlsli"

float4 main(Output input) : SV_TARGET
{
    //ゲームボーイ風
    //const float dx = 1.00f / 1280.00f;
    //const float dy = 1.00f / 720.00f;
    //float pixelSizeX = dx * value;
    //float pixelSizeY = dy * value;
    //float ratioX = (int)(input.uv.x / pixelSizeX) * pixelSizeX;
    //float ratioY = (int)(input.uv.y / pixelSizeY) * pixelSizeY;
    float4 col = tex0.Sample(smp, input.uv);
    //col = dot(col.rgb, float3(0.3, 0.59, 0.11));
    if (col.r <= 0.25f)
    {
        col = color1;
    }
    else if (col.r <= 0.50f)
    {
        col = color2;
    }
    else if (col.r <= 0.75f)
    {
        col = color3;
    }
    else
    {
        col = color4;
    }
    return col;
}