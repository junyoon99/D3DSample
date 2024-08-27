#define WIN32_LEAN_AND_MEAN // ����� ������� ����� ����

#include <Windows.h>
#include <wrl/client.h>
#include <d3d11.h>

#pragma comment (lib, "d3d11.lib")

using namespace Microsoft::WRL;

const wchar_t CLASS_NAME[]{ L"MyClassName" };
const wchar_t TITLE_TEXT[]{ L"Direct3D Sample" };
const int WINDOW_WIDTH{ 800 };
const int WINDOW_HEIGHT{ 600 };

HWND gHwnd{};
HINSTANCE gInstance{};

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

// Direct3D
// 1. ����̽� ���� (����̽� ���ؽ�Ʈ ����)
// 2. ����ü�� ����
// 3. ����Ÿ������ �ĸ� ���� ����
// 4. ���̹��� + ���ٽǹ��� ����
// 5. ���������� ����
// 6. ����Ʈ ���� View port


// 7. �׸���
	// ����Ÿ��
	// ���̽��ٽ� ����

ComPtr<IDXGISwapChain> gspSwapChain{}; // Interface DX Graphic Infrastructure
ComPtr<ID3D11Device> gspDevice{};
ComPtr<ID3D11DeviceContext> gspDeviceContext;

ComPtr<ID3D11Texture2D> gspRenderTarget{};
ComPtr<ID3D11Texture2D> gspDepthStencil{};
ComPtr<ID3D11RenderTargetView> gspRenderTargetView{};
ComPtr<ID3D11DepthStencilView> gspDepthStencilView{};


void InitD3D();
void DestroyD3D();

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nShowCmd) 
{
	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(WNDCLASSEX));
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpszClassName = CLASS_NAME;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wc.lpfnWndProc = WindowProc;
	wc.cbSize = sizeof(WNDCLASSEX);

	if (!RegisterClassEx(&wc)) 
	{
		MessageBox(nullptr, L"Failed to RegistaerClassEx()", L"Error", MB_OK);
		return 0;
	}

	gInstance = hInstance;

	RECT wr{ 0, 0, WINDOW_WIDTH, WINDOW_HEIGHT };
	AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, FALSE);

	gHwnd = CreateWindowEx(
		0,
		CLASS_NAME,
		TITLE_TEXT,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT,
		wr.right - wr.left, wr.bottom - wr.top,
		NULL, NULL,
		gInstance,
		NULL
	);

	if (!gHwnd) 
	{
		MessageBox(nullptr, L"Failed to CreateWindowEx()", L"Error", MB_OK);
		return 0;
	}

	ShowWindow(gHwnd, nShowCmd);
	SetForegroundWindow(gHwnd);
	SetFocus(gHwnd);
	UpdateWindow(gHwnd);

	InitD3D();

	MSG msg{};
	while (true) 
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);

			if (msg.message == WM_QUIT) 
			{
				break;
			}
		}
		else 
		{
			//GameLoop
		}
	}
	DestroyD3D();

	return (int)msg.wParam;
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		case WM_CLOSE:
			DestroyWindow(hwnd);
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		default:
			return DefWindowProc(hwnd, message, wParam, lParam);
	}

	return 0;
}

void InitD3D()
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = gHwnd;
	scd.SampleDesc.Count = 1;
	scd.Windowed = TRUE;

	D3D11CreateDeviceAndSwapChain(
		NULL,
		D3D_DRIVER_TYPE_HARDWARE,
		NULL,
		0,
		NULL, // {9.0, 10.0, 11.0}������ ȣȯ�Ǵ� ������ �迭�� �Ѱ��� 
		0,	// �迭�� ����
		D3D11_SDK_VERSION,
		&scd,
		gspSwapChain.ReleaseAndGetAddressOf(),
		gspDevice.ReleaseAndGetAddressOf(),
		NULL,
		gspDeviceContext.ReleaseAndGetAddressOf()
	);

	// RenderTarget
	gspSwapChain->GetBuffer(0, IID_PPV_ARGS(gspRenderTarget.ReleaseAndGetAddressOf()));
	gspDevice->CreateRenderTargetView(
		gspRenderTarget.Get(),
		nullptr,
		gspRenderTargetView.ReleaseAndGetAddressOf()
	);

	// DepthStencil
	CD3D11_TEXTURE2D_DESC td(
		DXGI_FORMAT_D24_UNORM_S8_UINT,
		WINDOW_WIDTH,
		WINDOW_HEIGHT,
		1,
		1,
		D3D11_BIND_DEPTH_STENCIL
	);
	gspDevice->CreateTexture2D(&td, nullptr, gspDepthStencil.ReleaseAndGetAddressOf());

	CD3D11_DEPTH_STENCIL_VIEW_DESC dsvd(
		D3D11_DSV_DIMENSION_TEXTURE2D
	);
	gspDevice->CreateDepthStencilView(
		gspDepthStencil.Get(),
		&dsvd,
		gspDepthStencilView.ReleaseAndGetAddressOf()
	);


}

void DestroyD3D()
{
	gspSwapChain.Reset();
	gspDevice.Reset();
	gspDeviceContext.Reset();

	DestroyWindow(gHwnd);
	UnregisterClass(CLASS_NAME, gInstance);
}
