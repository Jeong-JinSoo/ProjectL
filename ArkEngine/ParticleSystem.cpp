#include <d3d11.h>
#include "ResourceManager.h"
#include "ArkDevice.h"
#include "Camera.h"
#include "ArkEffect.h"
#include "d3dx11effect.h"
#include "ParticleEffect.h"
#include "DXTK/DirectXTex.h"

#include "ParticleSystem.h"

ArkEngine::ArkDX11::ParticleSystem::ParticleSystem()
	:_maxParticles(0), _firstRun(false),
	_gameTime(0), _timeStep(0), _age(0),
	_initVB(nullptr), _drawVB(nullptr), _streamOutVB(nullptr),
	_texArraySRV(nullptr), _randomTexSRV(nullptr)
{
	_eyePosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitPosW = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	_emitDirW = DirectX::XMFLOAT3(0.0f, 1.0f, 0.0f);
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

void ArkEngine::ArkDX11::ParticleSystem::Initialize(ID3D11Device* device, ID3D11ShaderResourceView* texArraySRV, ID3D11ShaderResourceView* randomTexSRV, unsigned int maxParticle)
{
	//maxParticle = _maxParticles;
	//_arkEffect = new ArkEffect();

	_maxParticles = maxParticle;
	_texArraySRV = texArraySRV;
	_randomTexSRV = randomTexSRV;

	BuildVB(device);
	SetEffect();
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
	SetViewProj(VP);
	SetGameTime(_gameTime);
	SetTimeStep(_timeStep);
	SetEyePosW(_eyePosW);
	SetEmitPosW(_emitPosW);
	SetEmitDirW(_emitDirW);
	SetTexArray(_texArraySRV);
	SetRandomTex(_randomTexSRV);

	// �Է� ������ �ܰ踦 ����
	dc->IASetInputLayout(_arkEffect->GetInputLayOut());
	dc->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_POINTLIST);
	unsigned int stride = sizeof(Particle);
	unsigned int offset = 0;

	// ���� �����̸� �ʱ�ȭ�� ���� ���۸� ����ϰ�, �׷��� �ʴٸ�
	// ������ ���� ����� ���� ���� ���۸� ����Ѵ�
	if (_firstRun)
	{
		// _initVB�� ���� CPU�� �ö󰡼� ���������� �ҷ��´�
		// �������� ���� ����
		dc->IASetVertexBuffers(0, 1, &_initVB, &stride, &offset);
	}
	else
	{
		dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);
	}

	// ���� ���� ����� ��Ʈ�� ��� ���� ������� �׷��� ���ڵ��� �����Ѵ�
	// ���ŵ� ���ڵ��� ��Ʈ�� ����� ���ؼ� ��� ���� ���ۿ� ��ϵȴ�
	// SO�ܰ迡 ���ؽ� ���۸� _streamOutVB�� ���ε����ִ� �Լ�
	dc->SOSetTargets(1, &_streamOutVB, &offset);

	D3DX11_TECHNIQUE_DESC techDesc;
	 _streamOutTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_streamOutTech->GetPassByIndex(p)->Apply(0, dc);

		if (_firstRun)
		{
			// ���������� �ϳ��̱⶧���� ������ 1���� ������ �ִ�
			dc->Draw(1, 0);
			_firstRun = false;
		}
		else
		{
			// ���� ������ ��Ȯ�� �� �� ���� ������
			dc->DrawAuto();
		}
	}

	ID3D11Buffer* bufferArray[1] = { 0 };
	dc->SOSetTargets(1, bufferArray, &offset);

	std::swap(_drawVB, _streamOutVB);

	dc->IASetVertexBuffers(0, 1, &_drawVB, &stride, &offset);

	_drawTech->GetDesc(&techDesc);
	for (unsigned int p = 0; p < techDesc.Passes; ++p)
	{
		_drawTech->GetPassByIndex(p)->Apply(0, dc);

		dc->DrawAuto();
	}
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::ParticleSystem::CreateRandomTextureSRV(ID3D11Device* device)
{
	// ���� �����͸� �����Ѵ�
	std::vector<DirectX::XMFLOAT4> randomValues(1024);

	for (int i = 0; i < 1024; i++)
	{
		randomValues[i].x = GetRandomFloat(-1.0f, 1.0f);
		randomValues[i].y = GetRandomFloat(-1.0f, 1.0f);
		randomValues[i].z = GetRandomFloat(-1.0f, 1.0f);
		randomValues[i].w = GetRandomFloat(-1.0f, 1.0f);
	}

	D3D11_SUBRESOURCE_DATA initData;
	initData.pSysMem = randomValues.data();
	initData.SysMemPitch = 1024 * sizeof(DirectX::XMFLOAT4);
	initData.SysMemSlicePitch = 0;

	// ���ο� �ؽ��ĸ� �����Ѵ�
	D3D11_TEXTURE1D_DESC texDesc;
	texDesc.Width = 1024;
	texDesc.MipLevels = 1;
	texDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	texDesc.Usage = D3D11_USAGE_IMMUTABLE;
	texDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags = 0;
	texDesc.MiscFlags = 0;
	texDesc.ArraySize = 1;

	ID3D11Texture1D* randomTex;
	device->CreateTexture1D(&texDesc, &initData, &randomTex);

	// ResourceView�� �����Ѵ�
	D3D11_SHADER_RESOURCE_VIEW_DESC viewDesc;
	viewDesc.Format = texDesc.Format;
	viewDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE1D;
	viewDesc.Texture1D.MipLevels = texDesc.MipLevels;
	viewDesc.Texture1D.MostDetailedMip - 0;

	ID3D11ShaderResourceView* randomTexSRV = 0;
	device->CreateShaderResourceView(randomTex, &viewDesc, &randomTexSRV);

	return randomTexSRV;
}

ID3D11ShaderResourceView* ArkEngine::ArkDX11::ParticleSystem::CreateTexture2DArraySRV(ID3D11Device* device, ID3D11DeviceContext* dc, std::vector<std::wstring>& filenames)
{
	std::vector<DirectX::TexMetadata> metadata(filenames.size());
	std::vector<DirectX::ScratchImage> scratchImage(filenames.size());
	HRESULT hr;

	// �̹������� �ε��Ѵ�
	for (int i = 0; i < filenames.size(); i++)
	{
		hr = LoadFromDDSFile(filenames[i].c_str(), DirectX::DDS_FLAGS_NONE, &metadata[i], scratchImage[i]);
		if (FAILED(hr))
		{
			return nullptr;
		}
	}

	// �ؽ�ó �迭�� �����Ѵ�
	D3D11_TEXTURE2D_DESC texArrayDesc;
	ZeroMemory(&texArrayDesc, sizeof(D3D11_TEXTURE2D_DESC));
	texArrayDesc.Width = static_cast<UINT>(metadata[0].width);
	texArrayDesc.Height = static_cast<UINT>(metadata[0].height);
	texArrayDesc.MipLevels = metadata[0].mipLevels;
	texArrayDesc.ArraySize = static_cast<UINT>(filenames.size());
	texArrayDesc.Format = metadata[0].format;
	texArrayDesc.SampleDesc.Count = 1;
	texArrayDesc.SampleDesc.Quality = 0;
	texArrayDesc.Usage = D3D11_USAGE_DEFAULT;
	texArrayDesc.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	texArrayDesc.CPUAccessFlags = 0;
	texArrayDesc.MiscFlags = 0;


	ID3D11Texture2D* textureArray = nullptr;
	hr = device->CreateTexture2D(&texArrayDesc, nullptr, &textureArray);
	if (FAILED(hr)) {
		return nullptr; // �ؽ�ó ���� ����
	}

	// ���� �̹������� �ؽ�ó �迭�� �����Ѵ�
	for (UINT i = 0; i < filenames.size(); ++i) {
		for (UINT mipLevel = 0; mipLevel < metadata[i].mipLevels; ++mipLevel) {
			const DirectX::Image* img = scratchImage[i].GetImage(mipLevel, 0, 0);
			dc->UpdateSubresource(
				textureArray,
				D3D11CalcSubresource(mipLevel, i, texArrayDesc.MipLevels),
				nullptr,
				img->pixels,
				img->rowPitch,
				img->slicePitch
			);
		}
	}

	// SRV�� �����Ѵ�
	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(D3D11_SHADER_RESOURCE_VIEW_DESC));
	srvDesc.Format = texArrayDesc.Format;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2DARRAY;
	srvDesc.Texture2DArray.MostDetailedMip = 0;
	srvDesc.Texture2DArray.MipLevels = texArrayDesc.MipLevels;
	srvDesc.Texture2DArray.FirstArraySlice = 0;
	srvDesc.Texture2DArray.ArraySize = static_cast<UINT>(filenames.size());

	ID3D11ShaderResourceView* srv;
	hr = device->CreateShaderResourceView(textureArray, &srvDesc, &srv);
	textureArray->Release(); // �ؽ�ó �迭 ����� �������Ƿ� �޸� ����

	if (FAILED(hr)) {
		return nullptr; // SRV ���� ����
	}

	return srv; // ������ Shader Resource View ��ȯ
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

void ArkEngine::ArkDX11::ParticleSystem::SetEffect()
{
	_arkEffect = ResourceManager::GetInstance()->GetResource<ArkEngine::ArkDX11::ArkEffect>("Resources/FX/ParticleFire.fx");
	auto effect = _arkEffect->GetEffect();

	_streamOutTech = effect->GetTechniqueByName("StreamOutTech");
	_drawTech = effect->GetTechniqueByName("DrawTech");

	_viewProjEffect = effect->GetVariableByName("gViewProj")->AsMatrix();
	_gameTimeEffect = effect->GetVariableByName("gGameTime")->AsScalar();
	_timeStepEffect = effect->GetVariableByName("gTimeStep")->AsScalar();
	_eyePosWEffect = effect->GetVariableByName("gEyePosW")->AsVector();
	_emitPosWEffect = effect->GetVariableByName("gEmitPosW")->AsVector();
	_emitDirWEffect = effect->GetVariableByName("gEmitDirW")->AsVector();
	_texArray = effect->GetVariableByName("gTexArray")->AsShaderResource();
	_randomTex = effect->GetVariableByName("gRandomTex")->AsShaderResource();
}

float ArkEngine::ArkDX11::ParticleSystem::GetRandomFloat(float minNum, float maxNum)
{
	std::random_device rd;
	std::mt19937 generator(rd());
	std::uniform_int_distribution<> dist(minNum, maxNum);

	return dist(generator);
}

void ArkEngine::ArkDX11::ParticleSystem::SetViewProj(DirectX::CXMMATRIX m)
{
	_viewProjEffect->SetMatrix(reinterpret_cast<const float*>(&m));
}

void ArkEngine::ArkDX11::ParticleSystem::SetGameTime(float f)
{
	_gameTimeEffect->SetFloat(f);
}

void ArkEngine::ArkDX11::ParticleSystem::SetTimeStep(float f)
{
	_timeStepEffect->SetFloat(f);
}

void ArkEngine::ArkDX11::ParticleSystem::SetEyePosW(const DirectX::XMFLOAT3& v)
{
	_eyePosWEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitPosW(const DirectX::XMFLOAT3& v)
{
	_emitPosWEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetEmitDirW(const DirectX::XMFLOAT3& v)
{
	_emitDirWEffect->SetRawValue(&v, 0, sizeof(DirectX::XMFLOAT3));
}

void ArkEngine::ArkDX11::ParticleSystem::SetTexArray(ID3D11ShaderResourceView* tex)
{
	_texArray->SetResource(tex);
}

void ArkEngine::ArkDX11::ParticleSystem::SetRandomTex(ID3D11ShaderResourceView* tex)
{
	_randomTex->SetResource(tex);
}
