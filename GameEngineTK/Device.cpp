#include "Device.h"

// ハイブリッドグラフィックシステムに対しデフォルトで個別に優先する方を示す
// Indicates to hybrid graphics systems to prefer the discrete part by default
//extern "C"
//{
//	__declspec(dllexport) DWORD NvOptimusEnablement = 0x00000001;
//	__declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
//}

// シングルトンのためのポインタ変数を初期化する
std::unique_ptr<Devices> Devices::devices(nullptr);

// デバイスを生成する
void Devices::CreateDevice()
{
	UINT creationFlags = 0;

#ifdef _DEBUG
	creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	static const D3D_FEATURE_LEVEL featureLevels[] = {
		// TODO: サポートされるDirect3D機能レベルを修正する(11.1 フォルバックハンドリングに関連したコードを参考).
		//Modify for supported Direct3D feature levels (see code below related to 11.1 fallback handling).
		D3D_FEATURE_LEVEL_11_1,
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0,
		D3D_FEATURE_LEVEL_9_3,
		D3D_FEATURE_LEVEL_9_2,
		D3D_FEATURE_LEVEL_9_1,
	};

	// DX11 APIデバイスオブジェクトを生成し対応するコンテキストを取得する
	// Create the DX11 API device object, and get a corresponding context.
	HRESULT hr = D3D11CreateDevice(
		nullptr,                 // 規定のアダプタを使用するためにnullptrを指定する specify nullptr to use the default adapter
		D3D_DRIVER_TYPE_HARDWARE,
		nullptr,
		creationFlags,
		featureLevels,
		_countof(featureLevels),
		D3D11_SDK_VERSION,
		this->device.ReleaseAndGetAddressOf(),   // 生成されたDirect3Dデバイスを返す returns the Direct3D device created
		&this->featureLevel,                     // 生成されたデバイスの機能レベルを返す returns feature level of device created
		this->context.ReleaseAndGetAddressOf()   // デバイスイミディエイトコンテキストを返す returns the device immediate context
	);

	if (hr == E_INVALIDARG) {
		// DirectX 11.0プラットフォームがD3D_FEATURE_LEVEL_11_1を認識しない場合、認識されるまで再試行する必要がある
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
				// TODO: 必要に応じてメッセージIDを追加する Add more message IDs here as needed.
			};
			D3D11_INFO_QUEUE_FILTER filter = {};
			filter.DenyList.NumIDs = _countof(hide);
			filter.DenyList.pIDList = hide;
			d3dInfoQueue->AddStorageFilterEntries(&filter);
		}
	}
#endif
	// DirectX 11.1が存在する場合 DirectX 11.1 if present
	if (SUCCEEDED(this->device.As(&this->device1)))
		(void)this->context.As(&this->context1);

	// TODO: デバイスに依存したオブジェクトを初期化する 
	// Initialize device dependent objects here (independent of window size).
}

// SizeChangedイベントでウィンドウを変更するすべてのメモリリソースを配置する
// Allocate all memory resources that change on a window SizeChanged event.
void Devices::CreateResources()
{
	// 直前のウィンドウサイズを指定されたコンテキストでクリアする Clear the previous window size specific context.
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

	// スワップチェインが既に存在する場合サイズを変更するかスワップチェインを生成する
	// If the swap chain already exists, resize it, otherwise create one.
	if (this->swapChain) {
		HRESULT hr = this->swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

		if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET) {
			// デバイスが何らかの理由で削除された場合新しいデバイスとスワップチェインを生成する必要がある
			// If the device was removed for any reason, a new device and swap chain will need to be created.
			OnDeviceLost();
			// すべてがセットアップされた。この関数での実行を継続しない
			// OnDeviceLostがこの関数を再度呼び出し、正しく新デバイスの再設定を可能にする
			// Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
			// and correctly set up the new device.
			return;
		}
		else {
			ThrowIfFailed(hr);
		}
	}
	else {
		// まず、D3DデバイスからDirectX Graphics Interface(DXGI)デバイスを検索する First, retrieve the underlying DXGI Device from the D3D Device.
		ComPtr<IDXGIDevice1> dxgiDevice;
		ThrowIfFailed(this->device.As(&dxgiDevice));

		// デバイスが実行中の物理アダプタ（GPUまたはカード）を識別する Identify the physical adapter (GPU or card) this device is running on.
		ComPtr<IDXGIAdapter> dxgiAdapter;
		ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

		// 生成されたファクトリオブジェクトを取得する And obtain the factory object that created it.
		ComPtr<IDXGIFactory1> dxgiFactory;
		ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

		ComPtr<IDXGIFactory2> dxgiFactory2;
		if (SUCCEEDED(dxgiFactory.As(&dxgiFactory2))) {
			// DirectX 11.1またはそれ以降 DirectX 11.1 or later

			// スワップチェインのためのディスクリプタを生成するCreate a descriptor for the swap chain.
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

			// Win32ウィンドウからスワップチェインを生成する Create a SwapChain from a Win32 window.
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

		// このテンプレートは排他的なフルスクリーンモードをサポートしない
		// DXGIに対してALT+ENTERショートカットの応答を防止する
		// This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
		ThrowIfFailed(dxgiFactory->MakeWindowAssociation(this->hWnd, DXGI_MWA_NO_ALT_ENTER));
	}

	// 最終的な3Dレンダーターゲットとなるウィンドウへのバックバッファを取得する
	// Obtain the backbuffer for this window which will be the final 3D rendertarget.
	ComPtr<ID3D11Texture2D> backBuffer;
	ThrowIfFailed(this->swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

	// バインド時に使用するレンダーターゲット上のビューインターフェースを生成する
	// Create a view interface on the rendertarget to use on bind.
	ThrowIfFailed(this->device->CreateRenderTargetView(backBuffer.Get(), nullptr, this->renderTargetView.ReleaseAndGetAddressOf()));

	// デプス/ステンシルバッファとして2-Dサーフェイスを配置し、バインド時に使用する
	// サーフェイス上のデプスステンシルビューを生成する
	// Allocate a 2-D surface as the depth/stencil buffer and
	// create a DepthStencil view on this surface to use on bind.
	CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

	ComPtr<ID3D11Texture2D> depthStencil;
	ThrowIfFailed(this->device->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

	CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
	ThrowIfFailed(this->device->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, this->depthStencilView.ReleaseAndGetAddressOf()));

	// TODO: ウィンドウサイズに依存したオブジェクトを初期化する Initialize windows-size dependent objects here.
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
	// デバイスを生成する Create Device
	CreateDevice();
	// リソースを生成する Create Resources
	CreateResources();
}
