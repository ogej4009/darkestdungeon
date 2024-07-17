struct VTX2D_IN
{
    float4 vPOS : POSITION;
    float4 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
};

struct VTX2D_OUT
{
    float4 vPOS : SV_Position;
    float2 vUV : TEXCOORD;
    float4 vCOLOR : COLOR;
};



//Vertex Shaders

cbuffer TRANSDATA : register(b0)
{
    /*row_major*/
    matrix W;
    matrix V;
    matrix P;
    matrix WV;
    matrix VP;
    matrix WVP;
}

cbuffer TEXTCUT : register(b1)
{
    float4 CutData;
}

//Pixel Shader

//cbuffer TEXFILTER : register(b0)
//{
//    matrix LaplaceFilter;
//}

cbuffer TEXCOLOR : register(b0)
{
    float4 TColor;
}

Texture2D SpriteTex : register(t0);
SamplerState LSMP : register(s0);
SamplerState PSMP : register(s1);

VTX2D_OUT VS_TEX(VTX2D_IN _In)
{
    
    VTX2D_OUT Out = (VTX2D_OUT) 0.0f;
    
    Out.vPOS = mul(_In.vPOS, WVP);
    Out.vCOLOR = _In.vCOLOR;
    //Out.vUV.xy = _In.vUV.xy;
    Out.vUV.x = CutData.x + (_In.vUV.x * CutData.z);
    Out.vUV.y = CutData.y + (_In.vUV.y * CutData.w);
    
    return Out;
}

// SV_Target 이게 무슨 뜻이냐면 0번 타겟에다가 쏴라.
float4 PS_TEX(VTX2D_OUT _In) : SV_Target
{
    //float result = 0;
    float4 TexColor = 0;
    //float4 MinColor = 1;
    //float uvX = 1.0f / 640.0f;
    //float uvY = 1.0f / 384.0f;
    
    //float2 UvChange = _In.vUV;
    //TexColor = m_Tex.Sample(PSMP, _In.vUV);
    
    //if (TexColor.w < 1.0f)
    //{
    //    for (int y = -1; y < 2; ++y)
    //    {
    //        for (int x = -1; x < 2; ++x)
    //        {
    //            UvChange.x = _In.vUV.x - x * uvX;
    //            UvChange.y = _In.vUV.y - y * uvY;
    //            //int2 offset = { x / 1.0f, y / 1.0f };
            
    //            TexColor = m_Tex.Sample(PSMP,UvChange);
                
    //            if (TexColor.w != 0)
    //            {
    //                return float4(1.0f, 0, 0, 1.0f);
    //            }
    //        }
    //    }
    //}
    
    
    TexColor = SpriteTex.Sample(PSMP, _In.vUV);
  
    _In.vCOLOR = TexColor * TColor;

    
    return _In.vCOLOR;
}

