#pragma once
#ifndef GRAPHICS
#define GRAPHICS



#include <windows.h>
#include <wrl/client.h>
#include <d3d11_1.h>
#include <DirectXMath.h>
#include <DirectXColors.h>

#include <algorithm>
#include <exception>
#include <memory>
#include <stdexcept>
#include <SimpleMath.h>
#include <wrl.h>
#include <SpriteBatch.h>



using namespace DirectX;
using namespace Microsoft::WRL;
using namespace std;


// Uncopyable�N���X
class Uncopyable {
protected:
	Uncopyable() = default;
	~Uncopyable() = default;
private:
	Uncopyable(const Uncopyable&) = delete;
	Uncopyable& operator =(const Uncopyable&) = delete;
};


class Devices:private Uncopyable
{
private:
	Devices() {}


public:
	//Devices�N���X�̃C���X�^���X���擾����
	static Devices& Get() {
		if (devices.get() == nullptr)
		{
			devices.reset(new Devices());
		}
		return *devices.get();
	}
//	~Devices() {};
	


	// �E�B���h�E�n���h�����擾���� Get hWnd
	HWND HWnd() {
		return hWnd;
	}
	// �E�B���h�E�n���h����ݒ肷�� Set hWnd
	void HWnd(HWND hWnd) {
		this->hWnd = hWnd;
	}
	// �E�B���h�E�����擾���� Get width
	int Width() {
		return width;
	}
	// �E�B���h�E����ݒ肷�� Set width
	void Width(int width) {
		this->width = width;
	}
	// �E�B���h�E�����擾���� Get height
	int Height() {
		return height;
	}
	// �E�B���h�E����ݒ肷�� Set height
	void Height(int height) {
		this->height = height;
	}
	// �f�o�C�X���擾���� Get device object
	ComPtr<ID3D11Device> Device() {
		return this->device;
	}
	// �f�o�C�X��ݒ肷�� Set device object
	void Device(ComPtr<ID3D11Device> device) {
		this->device = device;
	}
	// �f�o�C�X�R���e�L�X�g���擾���� Get device context object
	ComPtr<ID3D11DeviceContext> Context() {
		return this->context;
	}
	// �f�o�C�X�R���e�L�X�g��ݒ肷�� Set device context object
	void Context(ComPtr<ID3D11DeviceContext> context) {
		this->context = context;
	}

	// �X���b�v�`�F�C�����擾���� Get SwapChain
	ComPtr<IDXGISwapChain> SwapChain() {
		return swapChain;
	}

	//�r���[�|�[�g���擾����
	CD3D11_VIEWPORT Viewport() { return viewport; }

	//�r���[�|�[�g��ݒ肷��
	void Viewport(CD3D11_VIEWPORT viewport) {this->viewport=viewport; }

	// �X���b�v�`�F�C����ݒ肷�� Set SwapChain
	void SwapChain(ComPtr<IDXGISwapChain> swapChain) {
		this->swapChain = swapChain;
	}
	// �����_�[�^�[�Q�b�g�r���[���擾����Get RenderTargetView
	ComPtr<ID3D11RenderTargetView> RenderTargetView() {
		return renderTargetView;
	}
	// �����_�[�^�[�Q�b�g�r���[��ݒ肷�� Set RenderTargetView
	void RenderTargetView(ComPtr<ID3D11RenderTargetView> renderTargetView) {
		this->renderTargetView = renderTargetView;
	}

	// �f�v�X�X�e���V���r���[���擾���� Get DepthStencilView
	ComPtr<ID3D11DepthStencilView> DepthStencilView() {
		return depthStencilView;
	}
	// �f�v�X�X�e���V���r���[��ݒ肷�� Set DepthStencilView
	void DepthStencilView(ComPtr<ID3D11DepthStencilView> depthStencilView) {
		this->depthStencilView = depthStencilView;
	}

	//�X�v���C�g�o�b�`���擾����
	std::unique_ptr<DirectX::SpriteBatch>& SpriteBatch() { return spriteBatch; }

	//�X�v���C�g�o�b�`��ݒ肷��
	void SpriteBatch(std::unique_ptr<DirectX::SpriteBatch>& spriteBatch) {
		this->spriteBatch = std::move(spriteBatch);
	}


	// �f�o�C�X�𐶐����� Create device
	void CreateDevice();

	// ���\�[�X�𐶐����� Create Resource
	void CreateResources();

	// �f�o�C�X���X�g�����������ꍇ OnDeviceLost
	void OnDeviceLost();

	// Throw exception of error
	void ThrowIfFailed(HRESULT hr) {
		if (FAILED(hr)) {
			// Set a breakpoint on this line to catch DirectX API errors
			throw std::exception();
		}
	}


private:
	static std::unique_ptr<Devices> devices;
	HWND hWnd;
	int  width;
	int  height;
	ComPtr<ID3D11Device> device;
	ComPtr<ID3D11Device1> device1;
	ComPtr<ID3D11DeviceContext>	context;
	ComPtr<ID3D11DeviceContext1> context1;
	D3D_FEATURE_LEVEL  featureLevel;

	ComPtr<IDXGISwapChain> swapChain;
	ComPtr<IDXGISwapChain1> swapChain1;
	ComPtr<ID3D11RenderTargetView> renderTargetView;
	ComPtr<ID3D11DepthStencilView> depthStencilView;

	//�r���[�|�[�g
	CD3D11_VIEWPORT  viewport;
	//�X�v���C�g�o�b�`
	std::unique_ptr<DirectX::SpriteBatch> spriteBatch;



};

#endif	// GRAPHICS
