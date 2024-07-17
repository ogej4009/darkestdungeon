//struct VTX2D_IN
//{
//    float4 vPOS : POSITION;
//}; VTX ���̴����� ���� input���� �ʿ����� �ʳ�����.

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

// �̷��� �־��ָ� ���̴����� ���� ����ü�� ���� �޾��� �ʿ䰡 ����.
VTX_GRIDOUT VS_GRID(float4 _In : POSITION)
{
    VTX_GRIDOUT Out = (VTX_GRIDOUT) 0.0f;
    Out.vPOS = mul(_In, WVP);
    Out.vWorld = mul(_In, W);
    
    return Out;
}

// �׸��� ���̴��� �������, �׸��巣������ �̳�Ŭ������
cbuffer HGRIDDATA : register(b1)
{
    float Step;
    float Borader;
    float MinA;
    float MaxA;
    float4 Color;
    float4 CamPos;
};


// SV_Target �̰� ���� ���̳ĸ� 0�� Ÿ�ٿ��ٰ� ����.
float4 PS_GRID(VTX_GRIDOUT _In) : SV_Target
{
    // �簢���� ��û���� Ŭ���̴�.
    // Out.vPOS WVP�� ������ �༮�� ���´�.
    // Out.vPOS ������ ���� �� �̻��� ���ü��� ����.
    // X = -1 ~ 1
    // Y = -1 ~ 1
    // Z = 0 ~ 1
    
    // 0~1f
    // ������ ���ü��� �ִ�.
    // ���밪���� �ٲ���� �Ѵ�.
    float CalPosX = abs(_In.vWorld.x % Step);
    float CalPosZ = abs(_In.vWorld.z % Step);
    
    float CalPosMaxX = abs(_In.vWorld.x % (Step * 10.0f));
    float CalPosMaxZ = abs(_In.vWorld.z % (Step * 10.0f));
    
    
    // 1000��������
    
    // �������� ���������� 
    // 1�� ������ �簢���� �����
    
    // �����ϱ� ���簪�̴�.
    // ���� �ܰ��� �簢�� �׳� ����ϸ� �ȴ�.
    
    
    // ū �ܰ��� �簢��
    if (CalPosMaxX <= Borader || CalPosMaxZ <= Borader)
    {
        return Color;
    }
    
    if (CalPosX <= Borader || CalPosZ <= Borader)
    {
        float4 CalColor = Color;
        CalColor.w = 1.0f;
        // �������� ���������� �����ؾ� �Ѵ�.
        //if (DisLen > 20.0f)
        //{
        //    CalColor.w = 0.0f;
        //}
        
        return CalColor;
    }
    
    // ���忡���� �ڽ��� �ȼ��� ��ġ�� ��ȯ�Ǽ� ���´�.
    // ������ ���� �ȴ�.
    
    clip(-1);

    float4 resultColor = float4(0.0f, 0.0f, 0.0f, 0.0f);
    return resultColor;
}

