struct VTX2D_IN
{
    float4 vPOS : POSITION;
};

struct VTX2D_OUT
{
    float4 vPOS : SV_Position;
};

// SPINE
//Vertex Shader

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


cbuffer DEBUGCOLOR : register(b0)
{
    float4 DEBUGCOLOR;
}

VTX2D_OUT VS_DEBUG(VTX2D_IN _In)
{

    VTX2D_OUT Out = (VTX2D_OUT) 0.0f;
    Out.vPOS = mul(_In.vPOS, WVP);
    
    return Out;
}

// SV_Target 이게 무슨 뜻이냐면 0번 타겟에다가 쏴라.
float4 PS_DEBUG(VTX2D_OUT _In) : SV_Target
{
    return DEBUGCOLOR;
}

