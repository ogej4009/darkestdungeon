//struct VTX2D_IN
//{
//    float4 vPOS : POSITION;
//}; VTX 쉐이더에서 따로 input값이 필요하지 않나보다.

struct VTX_GRIDOUT
{
    float4 vPOS : SV_Position;
    float4 vWorld : POSITION;
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

// 이렇게 넣어주면 쉐이더에서 굳이 구조체를 만들어서 받아줄 필요가 없다.
VTX_GRIDOUT VS_GRID(float4 _In : POSITION)
{
    VTX_GRIDOUT Out = (VTX_GRIDOUT) 0.0f;
    Out.vPOS = mul(_In, WVP);
    Out.vWorld = mul(_In, W);
    
    return Out;
}

// 그리드 쉐이더용 상수버퍼, 그리드랜더러의 이너클래스다
cbuffer HGRIDDATA : register(b1)
{
    float Step;
    float Borader;
    float MinA;
    float MaxA;
    float4 Color;
    float4 CamPos;
};


// SV_Target 이게 무슨 뜻이냐면 0번 타겟에다가 쏴라.
float4 PS_GRID(VTX_GRIDOUT _In) : SV_Target
{
    // 사각형이 엄청나게 클것이다.
    // Out.vPOS WVP가 곱해진 녀석이 들어온다.
    // Out.vPOS 범위가 절대 얼마 이상이 나올수가 없다.
    // X = -1 ~ 1
    // Y = -1 ~ 1
    // Z = 0 ~ 1
    
    // 0~1f
    // 음수가 나올수도 있다.
    // 절대값으로 바꿔줘야 한다.
    float CalPosX = abs(_In.vWorld.x % Step);
    float CalPosZ = abs(_In.vWorld.z % Step);
    
    float CalPosMaxX = abs(_In.vWorld.x % (Step * 10.0f));
    float CalPosMaxZ = abs(_In.vWorld.z % (Step * 10.0f));
    
    
    // 1000기준으로
    
    // 높아지면 높아질수록 
    // 1의 단위의 사각형을 만들고
    
    // 예상하기 힘든값이다.
    // 작은 단계의 사각형 그냥 출력하면 된다.
    
    
    // 큰 단계의 사각형
    if (CalPosMaxX <= Borader || CalPosMaxZ <= Borader)
    {
        return Color;
    }
    
    if (CalPosX <= Borader || CalPosZ <= Borader)
    {
        float4 CalColor = Color;
        CalColor.w = 1.0f;
        // 높아지면 높아질수록 증가해야 한다.
        //if (DisLen > 20.0f)
        //{
        //    CalColor.w = 0.0f;
        //}
        
        return CalColor;
    }
    
    // 월드에서의 자신의 픽셀의 위치로 변환되서 들어온다.
    // 보간된 값이 된다.
    
    clip(-1);

    float4 resultColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    return resultColor;
}

