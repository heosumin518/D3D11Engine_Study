#pragma once
#include "ResourceBase.h"

/// 어떠한 재질.
/// 쉐이더와 쉐이더와 더불어 등장하는 인자들을 같이 들고 있다.
/// 쉐이더와 관련된 인자들로 묶인 파일.
/// 쉐이더에 추가적으로 텍스처가 또는 인자들이 들어가 있는 것

class Shader;
class Texture;

class Material :
    public ResourceBase
{
    using Super = ResourceBase;
public:
    Material();
    virtual ~Material();

    shared_ptr<Shader> GetShader() { return _shader; }
    shared_ptr<Texture> GetTexture() { return _texture; }

    void SetShader(shared_ptr<Shader> shader) { _shader = shader; }
    void SetTexture(shared_ptr<Texture> texture) { _texture = texture; }

private:
    shared_ptr<Shader> _shader;

    // 쉐이더에 넘기는 온갖 인자들
    shared_ptr<Texture> _texture;

};

