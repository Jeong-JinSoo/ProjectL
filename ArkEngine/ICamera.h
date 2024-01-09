/// <summary>
/// 
/// ī�޶� ������ �� �ʼ� ��Ҹ� ���� �������̽�
/// 
/// 2023.09.26 YJH
/// </summary>

#pragma once
namespace ArkEngine
{
	class ICamera abstract
	{
	public:
		ICamera() {};
		virtual ~ICamera() {};

	public:
		virtual bool GetMain() abstract;
		virtual void SetMain(bool tf) abstract;

	public:
		virtual void Delete() abstract;

	public:
		virtual void SetProjectionMatrix(float fovY, float aspect, float nz, float fz, bool isPerspective) abstract;

		virtual void Strafe(float deltaTime) abstract;
		virtual void Walk(float deltaTime) abstract;
		virtual void UpDown(float deltaTime) abstract;
	};
}