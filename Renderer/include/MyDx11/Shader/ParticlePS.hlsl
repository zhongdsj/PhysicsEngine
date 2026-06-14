struct PSInput
{
    float4 pos : SV_POSITION;
    float4 color : COLOR0;
    float2 uv  : TEXCOORD0;
};

float4 main(PSInput input) : SV_TARGET
{
    // 计算当前片元到圆心的距离
    float dist = length(input.uv);

// 距离 > 1.0 就丢弃像素，形成圆形
if (dist > 1.0)
    discard;

return input.color;
}