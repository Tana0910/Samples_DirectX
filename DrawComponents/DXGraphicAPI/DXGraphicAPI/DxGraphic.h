#pragma once
// M_PI
#define _USE_MATH_DEFINES
#include <math.h>
#include <vector>
#include <fstream>
#include <Shlwapi.h>
#pragma comment(lib, "Shlwapi.lib")

// D3D
#include <d3d11.h>
#include <d3dcompiler.h>
#include <DirectXMath.h>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "D3DCompiler.lib")

// Comptr
#include <atlcomcli.h>

// Core Math Part
#include "Math\\Vector.h"

extern HINSTANCE g_hInstance;

namespace DXGraphicAPI
{
	class CDxGraphic
	{
	private:
		HWND m_WindowHandle = NULL;
		// �@�\���x��, �t�H�[�}�b�g
		D3D_FEATURE_LEVEL featurelevel = D3D_FEATURE_LEVEL_11_0;
		UINT swapchaincount = 1;
		DXGI_FORMAT swapchainformat = DXGI_FORMAT_B8G8R8A8_UNORM;
		DXGI_FORMAT depthstencilformat = DXGI_FORMAT_D24_UNORM_S8_UINT;
		DXGI_SAMPLE_DESC sampledesc = { 1, 0 };

		// �R�A�ƂȂ鏈�����s�����߂̕ϐ�
		CComPtr<ID3D11Device> device;
		CComPtr<ID3D11DeviceContext> context;
		CComPtr<IDXGISwapChain> swapchain;
		CComPtr<ID3D11Texture2D> backbuffer;
		CComPtr<ID3D11RenderTargetView> rtv;
		CComPtr<ID3D11Texture2D> depthtex;
		CComPtr<ID3D11DepthStencilView> dsv;
		CComPtr<ID3D11RasterizerState> rs;
		CComPtr<ID3D11DepthStencilState> dss;
		CComPtr<ID3D11VertexShader> vertexshader;
		CComPtr<ID3D11GeometryShader> geometryshader;
		CComPtr<ID3D11PixelShader> pixelshader;
		CComPtr<ID3D11InputLayout> inputlayout;

		// �萔�o�b�t�@
		CComPtr<ID3D11Buffer> matrixbuffer;

		
		CComPtr<ID3D11Buffer> vertexbuffer;
		CComPtr<ID3D11Buffer> indexbuffer;

		UINT numindices = 0;

		// DirectX�Z�p�p�}�g���b�N�X
		DirectX::XMMATRIX d3dworldmatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX d3dviewmatrix = DirectX::XMMatrixIdentity();
		DirectX::XMMATRIX d3dprojmatrix = DirectX::XMMatrixIdentity();

		// �J�����ʒu, �����_
		Math::Vector3 m_cameraposition = Math::Vector3();
		Math::Vector3 m_lookatpoint = Math::Vector3();

		// �J�����̏�����𔽓]������t���O
		bool m_camupset = false;

		struct Vertex
		{
			float position[3];  // (x, y, z)
			float color[4];     // (r, g, b, a)
		};

		struct Triangle
		{
			int indices[3];
		};

		std::vector<Vertex> InputData =
		{
			{{-1.0f, -1.0f,  1.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
			{{-1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
			{{ 1.0f, -1.0f, -1.0f}, {0.0f, 0.0f, 1.0f, 1.0f}},
			{{ 1.0f, -1.0f,  1.0f}, {1.0f, 1.0f, 0.0f, 1.0f}},
			{{-1.0f,  1.0f,  1.0f}, {0.0f, 1.0f, 1.0f, 1.0f}},
			{{-1.0f,  1.0f, -1.0f}, {1.0f, 0.0f, 1.0f, 1.0f}},
			{{ 1.0f,  1.0f, -1.0f}, {0.5f, 0.2f, 0.3f, 1.0f}},
			{{ 1.0f,  1.0f,  1.0f}, {0.0f, 0.9f, 0.1f, 1.0f}}
		};

		std::vector<Triangle> InputTriangle =
		{
			{{0, 1, 2}},
			{{3, 0, 2}},
			{{2, 6, 3}},
			{{6, 7, 3}},
			{{5, 7, 6}},
			{{4, 7, 5}},
			{{4, 5, 1}},
			{{4, 1, 0}},
			{{7, 4, 0}},
			{{0, 3, 7}},
			{{1, 6, 2}},
			{{5, 6, 1}}
		};

		struct CoordColor
		{
			DirectX::XMFLOAT3 coord;
			DirectX::XMFLOAT4 color;
		};

		struct MatrixBuffer
		{
			DirectX::XMMATRIX matproj;
			DirectX::XMMATRIX matview;
			DirectX::XMMATRIX matworld;
		};

		bool CreateDeviceAndSwapChain(int w, int h);

		bool CreateRenderTarget();

		bool CreateDefaultRasterizerState();

		bool CreateDepthStencilState();

		bool CreateStencilBuffer(int w, int h);

		bool CreateShaderFromCompiledFiles();

		bool CreateConstantBuffer();

		void ReleaseComPtr();

	public:
		CDxGraphic();
		~CDxGraphic();

		void SetWindowHandle(HWND hWnd);

		bool InitD3D(int w, int h);

		void Render();

		void LoadSampleData(int w, int h);

		void UpdateMatrices(int w, int h);

		bool ResizeView(int w, int h);
	};
}
