float4 PS_main(float2 texCoord : TEXCOORD0) : COLOR0
{
    // 深度バッファをサンプリング
    float depth = tex2D(samplerDepth, texCoord).r;
    
    // 現在のピクセルの深度と比較
    if (depth < currentDepth)
    {
        // 影の色を返す
        return float4(0.2, 0.2, 0.2, 1.0); // 灰色
    }
    else
    {
        // 通常の色を返す
        return float4(1.0, 1.0, 1.0, 1.0); // 白色
    }
}