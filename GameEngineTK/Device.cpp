#include "Device.h"

// �n�C�u���b�h�O���t�B�b�N�V�X�e���ɑ΂��f�t�H���g�ŌʂɗD�悷���������
// Indicates to hybrid graphics systems to prefer the discrete part by default
//extern "C"
//{
//	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}

// �V���O���g���̂��߂̃|�C���^�ϐ�������������
std::unique_ptr<Devices> Devices::devices(nullptr);

// �f�o�C�X�𐶐�����
void Devices::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] = {
		// TODO: �T�|�[�g�����Direct3D�@�\���x�����C������(11.1 �t�H���o�b�N�n���h�����O�Ɋ֘A�����R�[�h���Q�l).
		//Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// DX11 API�f�o�C�X�I�u�W�F�N�g�𐶐����Ή�����R���e�L�X�g���擾����
	// Create the DX11 API device object, and get a corresponding context.
	HRESULT hr = D3D11CreateDevice(
		nullptr,                 // �K��̃A�_�v�^���g�p���邽�߂�nullptr���w�肷�� specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		this->device.ReleaseAndGetAddressOf(),   // �������ꂽDirect3D�f�o�C�X��Ԃ� returns the Direct3D device created
		&this->featureLevel,                     // �������ꂽ�f�o�C�X�̋@�\���x����Ԃ� returns feature level of device created
		this->context.ReleaseAndGetAddressOf()   // �f�o�C�X�C�~�f�B�G�C�g�R���e�L�X�g��Ԃ� returns the device immediate context
	);

	if (hr == E_INVALIDARG) {
		// DirectX 11.0�v���b�g�t�H�[����D3D_FEATURE_LEVEL_11_1��F�����Ȃ��ꍇ�A�F�������܂ōĎ��s����K�v������
		// DirectX 11.0 platforms will not recognize D3D_FEATURE_LEVEL_11_1 so we need to retry without it.
		hr = D3D11CreateDevice(nullptr,
			D3D_DRIVER_TYPE_HARDWARE,
			nullptr,
			creationFlags,
			&featureLevels[1],
			_countof(featureLevels) - 1,
			D3D11_SDK_VERSION,
			this->device.ReleaseAndGetAddressOf(),
			&this->featureLevel,
			this->context.ReleaseAndGetAddressOf()
		);
	}

	ThrowIfFailed(hr);

#ifndef NDEBUG
	ComPtr<ID3D11Debug> d3dDebug;
	if (SUCCEEDED(this->device.As(&d3dDebug))) {
		ComPtr<ID3D11InfoQueue> d3dInfoQueue;
		if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue))) {
#ifdef _DEBUG
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
			d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
			D3D11_MESSAGE_ID hide[] = {
				D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
				// TODO: �K�v�ɉ����ă��b�Z�[�WID��ǉ����� Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif
	// DirectX 11.1�����݂���ꍇ DirectX 11.1 if present
	if (SUCCEEDED(this->device.As(&this->device1)))
		(void)this->context.As(&this->context1);

	// TODO: �f�o�C�X�Ɉˑ������I�u�W�F�N�g������������ 
	// Initialize device dependent objects here (independent of window size).
}

// SizeChanged�C�x���g�ŃE�B���h�E��ύX���邷�ׂẴ��������\�[�X��z�u����
// Allocate all memory resources that change on a window SizeChanged event.
void Devices::CreateResources()
{
	// ���O�̃E�B���h�E�T�C�Y���w�肳�ꂽ�R���e�L�X�g�ŃN���A���� Clear the previous window size specific context.
	ID3D11RenderTargetView* nullViews[] = { nullptr };
	this->context->OMSetRenderTargets(_countof(nullViews), nullViews, nullptr);
	this->renderTargetView.Reset();
	this->depthStencilView.Reset();
	this->context->Flush();

	UINT backBufferWidth = static_cast<UINT>(this->width);
	UINT backBufferHeight = static_cast<UINT>(this->height);
	DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
	DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
	UINT backBufferCount = 2;

	// �X���b�v�`�F�C�������ɑ��݂���ꍇ�T�C�Y��ύX���邩�X���b�v�`�F�C���𐶐�����
	// If the swap chain already exists, resize it, otherwise create one.
	if (this->swapChain) {
		HRESULT hr = this->swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			// �f�o�C�X�����炩�̗��R�ō폜���ꂽ�ꍇ�V�����f�o�C�X�ƃX���b�v�`�F�C���𐶐�����K�v������
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();
			// ���ׂĂ��Z�b�g�A�b�v���ꂽ�B���̊֐��ł̎��s���p�����Ȃ�
			// OnDeviceLost�����̊֐����ēx�Ăяo���A�������V�f�o�C�X�̍Đݒ���\�ɂ���
			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else {
			ThrowIfFailed(hr);
		}
	}
	else {
		// �܂��AD3D�f�o�C�X����DirectX Graphics Interface(DXGI)�f�o�C�X���������� First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		ThrowIfFailed(this->device.As(&dxgiDevice));

		// �f�o�C�X�����s���̕����A�_�v�^�iGPU�܂��̓J�[�h�j�����ʂ��� Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// �������ꂽ�t�@�N�g���I�u�W�F�N�g���擾���� And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2))) {
			// DirectX 11.1�܂��͂���ȍ~ DirectX 11.1 or later

			// �X���b�v�`�F�C���̂��߂̃f�B�X�N���v�^�𐶐�����Create a descriptor for the swap chain.
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc = { 0 };
			swapChainDesc.Width = backBufferWidth;
			swapChainDesc.Height = backBufferHeight;
			swapChainDesc.Format = backBufferFormat;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.BufferCount = backBufferCount;

			DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = { 0 };
			fsSwapChainDesc.Windowed = TRUE;

			// Win32�E�B���h�E����X���b�v�`�F�C���𐶐����� Create a SwapChain from a Win32 window.
			ThrowIfFailed(dxgiFactory2->CreateSwapChainForHwnd(
				this->device.Get(),
				this->hWnd,
				&swapChainDesc,
				&fsSwapChainDesc,
				nullptr,
				this->swapChain1.ReleaseAndGetAddressOf()
			));

			ThrowIfFailed(this->swapChain1.As(&this->swapChain));
		}
		else {
			DXGI_SWAP_CHAIN_DESC swapChainDesc = { 0 };
			swapChainDesc.BufferCount = backBufferCount;
			swapChainDesc.BufferDesc.Width = backBufferWidth;
			swapChainDesc.BufferDesc.Height = backBufferHeight;
			swapChainDesc.BufferDesc.Format = backBufferFormat;
			swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
			swapChainDesc.OutputWindow = this->hWnd;
			swapChainDesc.SampleDesc.Count = 1;
			swapChainDesc.SampleDesc.Quality = 0;
			swapChainDesc.Windowed = TRUE;

			ThrowIfFailed(dxgiFactory->CreateSwapChain(this->device.Get(), &swapChainDesc, this->swapChain.ReleaseAndGetAddressOf()));
		}

		// ���̃e���v���[�g�͔r���I�ȃt���X�N���[�����[�h���T�|�[�g���Ȃ�
		// DXGI�ɑ΂���ALT+ENTER�V���[�g�J�b�g�̉�����h�~����
		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		ThrowIfFailed(dxgiFactory->MakeWindowAssociation(this->hWnd, DXGI_MWA_NO_ALT_ENTER));
	}

	// �ŏI�I��3D�����_�[�^�[�Q�b�g�ƂȂ�E�B���h�E�ւ̃o�b�N�o�b�t�@���擾����
	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(this->swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// �o�C���h���Ɏg�p���郌���_�[�^�[�Q�b�g��̃r���[�C���^�[�t�F�[�X�𐶐�����
	// Create a view interface on the rendertarget to use on bind.
	ThrowIfFailed(this->device->CreateRenderTargetView(backBuffer.Get(), nullptr, this->renderTargetView.ReleaseAndGetAddressOf()));

	// �f�v�X/�X�e���V���o�b�t�@�Ƃ���2-D�T�[�t�F�C�X��z�u���A�o�C���h���Ɏg�p����
	// �T�[�t�F�C�X��̃f�v�X�X�e���V���r���[�𐶐�����
	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	ThrowIfFailed(this->device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	ThrowIfFailed(this->device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, this->depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: �E�B���h�E�T�C�Y�Ɉˑ������I�u�W�F�N�g������������ Initialize windows-size dependent objects here.
}

void Devices::OnDeviceLost() {
	this->depthStencilView.Reset();
	this->renderTargetView.Reset();
	this->swapChain1.Reset();
	this->swapChain.Reset();
	this->context1.Reset();
	this->context.Reset();
	this->device1.Reset();
	this->device.Reset();
	// �f�o�C�X�𐶐����� Create Device
	CreateDevice();
	// ���\�[�X�𐶐����� Create Resources
	CreateResources();
}
