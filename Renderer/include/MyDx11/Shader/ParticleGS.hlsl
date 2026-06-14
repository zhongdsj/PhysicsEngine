// 寄存器 b0 绑定 C++ 传来的 View * Proj 合并矩阵
cbuffer MatrixCB : register(b0)
{
    row_major matrix g_viewProj;
};

// 接收 VS 输出
struct GSInput
{
    float3 worldPos : TEXCOORD0;
    float4 color    : COLOR0;
    float size : TEXCOORD1;
};

// GS 输出：最终裁剪坐标使用 SV_POSITION（规范用法）
struct GSOutput
{
    float4 pos  : SV_POSITION;
    float4 color: COLOR0;
    float2 uv   : TEXCOORD0;
};


// 世界坐标 直接乘 ViewProj 合并矩阵，一步完成变换
float4 Transform(float3 worldP)
{
    return mul(float4(worldP, 1.0f), g_viewProj);
}

[maxvertexcount(4)]
void main(point GSInput input[1], inout TriangleStream<GSOutput> stream)
{
    GSOutput outVert;
    float3 center = input[0].worldPos;
    float s = input[0].size;

    // 世界空间固定 XY 面片，无广告牌
    float3 p0 = center + float3(-s, -s, 0.0f);
    float3 p1 = center + float3(-s, s, 0.0f);
    float3 p2 = center + float3(s, -s, 0.0f);
    float3 p3 = center + float3(s, s, 0.0f);

    float2 uv0 = float2(-1.0, -1.0);
    float2 uv1 = float2(-1.0, 1.0);
    float2 uv2 = float2(1.0, -1.0);
    float2 uv3 = float2(1.0, 1.0);

    outVert.pos = Transform(p0); outVert.color = input[0].color; outVert.uv = uv0; stream.Append(outVert);
    outVert.pos = Transform(p1); outVert.color = input[0].color; outVert.uv = uv1; stream.Append(outVert);
    outVert.pos = Transform(p2); outVert.color = input[0].color; outVert.uv = uv2; stream.Append(outVert);
    outVert.pos = Transform(p3); outVert.color = input[0].color; outVert.uv = uv3; stream.Append(outVert);
}