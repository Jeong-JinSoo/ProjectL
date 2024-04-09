#include <d3d11.h>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "Camera.h"
#include "ArkEffect.h"
#include "d3dx11effect.h"
#include "ParticleEffect.h"

#include "ParticleSystem.h"

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem(ID3D11Device* device, ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV, unsigned int maxParticle)
	:_maxParticles(0), _firstRun(false),
	_gameTime(0), _timeStep(0), _age(0),
	_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr),
	_texArraySRV(nullptr), _randomTexSRV(nullptr)
{
	_eyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitPosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitDirW = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
	
	_fx = new ParticleEffect();

	Initialize(device, _fx, texArraySRV, randomTexSRV, maxParticle);
}

ArkEngine::ArkDX11::ParticleSystem::~ParticleSystem()
{
}

float ArkEngine::ArkDX11::ParticleSystem::GetAge() const
{
	return _age;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEyePos(const DirectX::XMFLOAT3& eyePosW)
{
	_eyePosW = eyePosW;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitPos(const DirectX::XMFLOAT3& emitPosW)
{
	_emitPosW = emitPosW;
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitDir(const DirectX::XMFLOAT3& emitDirW)
{
	_emitDirW = emitDirW;
}

void ArkEngine::ArkDX11::ParticleSystem::Initialize(ID3D11Device* device, ParticleEffect* fx, ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV, unsigned int maxParticle)
{
	_maxParticles = maxParticle;

	_fx = fx;

	_texArraySRV = texArraySRV;
	_randomTexSRV = randomTexSRV;

	BuildVB(device);
}

void ArkEngine::ArkDX11::ParticleSystem::Reset()
{
	_firstRun = true;
	_age = 0.0f;
}

void ArkEngine::ArkDX11::ParticleSystem::Update(float deltaTime, float gameTime)
{
	_gameTime = gameTime;
	_timeStep = deltaTime;

	_age += deltaTime;
}

void ArkEngine::ArkDX11::ParticleSystem::Draw(ID3D11DeviceContext* dc, ArkEngine::ICamera* p_Camera)
{
	auto arkDevice = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkDevice>("Device");

	dc = arkDevice->GetDeviceContext();
	auto camera = static_cast<ArkEngine::ArkDX11::Camera*>(p_Camera);

	auto cameraView = camera->GetViewMatrix();
	auto cameraProj = camera->GetProjMatrix();

	DirectX::XMMATRIX VP = cameraView * cameraProj;

	// ������� �����Ѵ�
	_fx->SetViewProj(VP);
	_fx->SetGameTime(_gameTime);
	_fx->SetTimeStep(_timeStep);
	_fx->SetEyePosW(_eyePosW);
	_fx->SetEmitPosW(_emitPosW);
	_fx->SetEmitDirW(_emitDirW);
	_fx->SetTexArray(_texArraySRV);
	_fx->SetRandomTex(_randomTexSRV);

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
	//
// Create the buffer to kick-off the particle system.
//

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DEFAULT;
	vbd.ByteWidth = sizeof(Particle) * 1;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = 0;
	vbd.MiscFlags = 0;
	vbd.StructureByteStride = 0;

	// The initial particle emitter has type 0 and age 0.  The rest
	// of the particle attributes do not apply to an emitter.
	Particle p;
	ZeroMemory(&p, sizeof(Particle));
	p.Age = 0.0f;
	p.Type = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &p;

	device->CreateBuffer(&vbd, &vinitData, &_initVB);

	//
	// Create the ping-pong buffers for stream-out and drawing.
	//
	vbd.ByteWidth = sizeof(Particle) * _maxParticles;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER | D3D11_BIND_STREAM_OUTPUT;

	(device->CreateBuffer(&vbd, 0, &_drawVB));
	(device->CreateBuffer(&vbd, 0, &_streamOutVB));
}




