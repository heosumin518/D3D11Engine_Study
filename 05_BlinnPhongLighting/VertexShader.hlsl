#include "Shared.fxh"

/**
 * \brief Vertex Shader
 * \param input 
 * \return 
 */

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;
    output.m_position = mul(input.m_position, m_world);

    // 입사광 벡터와 카메라 벡터 구하기
    float3 lightDir = normalize(m_lightDir);    // 입사광 벡터 // 벡터의 길이를 1로 만든다 -> 정규화(normalize)
    float3 viewDir = normalize(output.m_position.xyz - m_eyePos.xyz);    // 시선 벡터 // 시선 위치로부터 현재위치까지의 방향벡터
    output.m_viewDir = viewDir;

    output.m_position = mul(output.m_position, m_view);
    output.m_position = mul(output.m_position, m_projection);

    float3 worldNormal = mul(input.m_normal, (float3x3) m_world);   // 법선 벡터 // 입력으로 들어온 로컬 공간에 있는 법선을 월드 공간으로 변환한다.
    worldNormal = normalize(worldNormal);
    output.m_worldNormal = worldNormal; // 흠흠..
	// 월드행렬을 float3x3 로 캐스팅을 하였는데, 4x4행렬에서 4번째 행 또는 열은 평행이동 값이므로 방향벡터에는 아무런 영향도 미치지 않는다.

    output.m_diffuse = dot(-lightDir, worldNormal); // 두 벡터의 내적을 구할 때 화살표의 밑동이 서로 만나기 위해 lightDir 를 음수값으로 두었다.

    if (m_useBlinnPhong == true)
    {
		float3 halfDir = normalize(lightDir + viewDir);     // 카메라 벡터와 입사광벡터의 중간을 의미하는 하프 벡터
        output.m_reflection = halfDir;
    }
    else
    {
		float3 reflectionDir = reflect(lightDir, worldNormal);  // 정반사광의 방향벡터
        output.m_reflection = reflectionDir;
    }

    output.m_uv = input.m_uv;   // 텍스처

    return output;
}

/*
 * 동일한 계산을 어느 쪽에서도 할 수 있다면 픽셀셰이더 보단 정점셰이더에서 하는 것이 성능상 유리하다.
 * 3D 연산을 할 때는 모든 변수들이 존재하는 공간이 일치해야 한다.
*/