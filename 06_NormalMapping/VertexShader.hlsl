#include "Shared.fxh"

/**
 * \brief Vertex Shader
 * \param input 
 * \return 
 */

VS_OUTPUT vs_main(VS_INPUT input)
{
    VS_OUTPUT output = (VS_OUTPUT) 0;

    // 정점 위치를 월드 공간으로 변환
    output.m_position = mul(input.m_position, m_world);
    output.m_uv = input.m_uv; // 텍스처 좌표

    // 카메라 벡터 구하기
	float3 viewDir = normalize(output.m_position.xyz - m_eyePos.xyz); // 시선 벡터(카메라 벡터) // 시선 위치(카메라 위치)로부터 현재위치까지의 방향벡터
    output.m_viewDir = viewDir;

    // 정점 위치를 뷰, 프로젝션 공간으로 변환
    output.m_position = mul(output.m_position, m_view);
    output.m_position = mul(output.m_position, m_projection);

    // 노말, 탄젠트 벡터를 월드공간으로 변환하여 픽셀셰이더에 전하기
    float3 worldNormal = mul(input.m_normal, (float3x3) m_world); // 법선 벡터 // 입력으로 들어온 로컬 공간에 있는 법선을 월드 공간으로 변환한다.
    output.N = normalize(worldNormal);

    float3 worldTangent = mul(input.m_tangent, (float3x3) m_world);
    output.T = normalize(worldTangent);

    return output;
}
/*
 * 위 코드에서 월드행렬을 float3x3 로 캐스팅을 하였는데,
 * 4x4행렬에서 4번째 행 또는 열은 평행이동 값이므로 방향벡터에는 아무런 영향도 미치지 않는다.
 */

/*
 * 동일한 계산을 어느 쪽에서도 할 수 있다면 픽셀셰이더 보단 정점셰이더에서 하는 것이 성능상 유리하다.
 * 3D 연산을 할 때는 모든 변수들이 존재하는 공간이 일치해야 한다.
*/

/*
 * BiTangent는 CrossProduct 계산 때문에 생략가능하다.
*/