#include <d3d11.h>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "Camera.h"
#include "ArkEffect.h"
#include "d3dx11effect.h"
#include "ParticleEffect.h"

#include "ParticleSystem.h"

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem()
	:_maxParticles(0), _firstRun(false), 
	_gameTime(0), _timeStep(0), _age(0),
	_eyePosW(0), _emitPosW(0), _emitDirW(0),
	_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr),
	_texArraySRV(nullptr), _randomTexSRV(nullptr)
{
}

ArkEngine::ArkDX11::ParticleSystem::~ParticleSystem()
{
}

float ArkEngine::ArkDX11::ParticleSystem::GetAge() const
{
	return 0.0f;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEyePos(const DirectX::XMFLOAT3& eyePosW)
{
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitPos(const DirectX::XMFLOAT3& emitPosW)
{
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitDir(const DirectX::XMFLOAT3& emitDirW)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Initialize(ID3D11Device* device, ParticleEffect* fx, ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV, unsigned int maxParticle)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Reset()
{
}

void ArkEngine::ArkDX11::ParticleSystem::Update(float deltaTime, float gameTime)
{
}

void ArkEngine::ArkDX11::ParticleSystem::Draw(ID3D11DeviceContext* dc, ArkEngine::ICamera* p_Camera)
{
	dc = _arkDevice->GetDeviceContext();
	auto camera = static_cast<ArkEngine::ArkDX11::Camera*>(p_Camera);

	auto cameraView = camera->GetViewMatrix();
	auto cameraProj = camera->GetProjMatrix();

	DirectX::XMMATRIX VP = cameraView * cameraProj;

	// ������� �����Ѵ�(�ϴ� ����)

	// �Է� ������ �ܰ踦 ����
	dc->IASetInputLayout(_arkEffect->GetInputLayOut());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	unsigned int stride = sizeof(Particle);
	unsigned int offset = 0;

	// ���� �����̸� �ʱ�ȭ�� ���� ���۸� ����ϰ�, �׷��� �ʴٸ�
	// ������ ���� ����� ���� ���� ���۸� ����Ѵ�
	if (_firstRun)
	{
		dc->IASetVertexBuffers(0, 1, &_initVB, &stride, &offset);
	}
	else
	{
		dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);
	}

	// ���� ���� ����� ��Ʈ�� ��� ���� ������� �׷��� ���ڵ��� �����Ѵ�
	// ���ŵ� ���ڵ��� ��Ʈ�� ����� ���ؼ� ��� ���� ���ۿ� ��ϵȴ�
	dc->SOSetTargets(1, &_streamOutVB, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	_fx->_streamOutTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_fx->_streamOutTech->GetPassByIndex(p)->Apply(0, dc);

		if (_firstRun)
		{
			dc->Draw(1, 0);
			_firstRun = false;
		}
		else
		{
			dc->DrawAuto();
		}
	}

	ID3D11Buffer* bufferArray[1] = { 0 };
	dc->SOSetTargets(1, bufferArray, &offset);

	std::swap(_drawVB, _streamOutVB);

	dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);

	_fx->_drawTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_fx->_drawTech->GetPassByIndex(p)->Apply(0, dc);

		dc->DrawAuto();
	}
}

void ArkEngine::ArkDX11::ParticleSystem::BuildVB(ID3D11Device* device)
{
}

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem(const ParticleSystem& rhs)
{
}


