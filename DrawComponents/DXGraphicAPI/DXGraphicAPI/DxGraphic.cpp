#pragma once
#include "pch.h"
#include "DxGraphic.h"

DXGraphicAPI::CDxGraphic::CDxGraphic()
{
}

DXGraphicAPI::CDxGraphic::~CDxGraphic()
{
}

bool DXGraphicAPI::CDxGraphic::CreateDeviceAndSwapChain(int w, int h)
{
	DXGI_SWAP_CHAIN_DESC desc = {
		{ static_cast<UINT>(w), static_cast<UINT>(h),{ 60, 1 },
		swapchainformat, DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED, DXGI_MODE_SCALING_UNSPECIFIED },
		sampledesc,
		DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_SHADER_INPUT,
		swapchaincount,
		m_WindowHandle,
		TRUE,
		DXGI_SWAP_EFFECT_DISCARD,
		DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH
	};

	if (FAILED(D3D11CreateDeviceAndSwapChain(
		nullptr,
		D3D_DRIVER_TYPE_HARDWARE,
		0,
		0,
		nullptr,
		0,
		D3D11_SDK_VERSION,
		&desc,
		&swapchain.p,
		&device.p,
		&featurelevel,
		&context)))
		return false;

	return true;
}

bool DXGraphicAPI::CDxGraphic::CreateRenderTarget()
{
	if (FAILED(swapchain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backbuffer)))
		return false;

	if (FAILED(device->CreateRenderTargetView(backbuffer, nullptr, &rtv)))
		return false;

	return true;
}

bool DXGraphicAPI::CDxGraphic::CreateDefaultRasterizerState()
{
	D3D11_RASTERIZER_DESC desc =
	{
		D3D11_FILL_SOLID, D3D11_CULL_BACK, TRUE, 0,	0.0f, 0.0f,
		TRUE, FALSE, FALSE, FALSE
	};
	if (FAILED(device->CreateRasterizerState(&desc, &rs))) return false;

	return true;
}

bool DXGraphicAPI::CDxGraphic::CreateDepthStencilState()
{
	D3D11_DEPTH_STENCIL_DESC desc =
	{
		TRUE, D3D11_DEPTH_WRITE_MASK_ALL, D3D11_COMPARISON_LESS,
		FALSE, D3D11_DEFAULT_STENCIL_READ_MASK, D3D11_DEFAULT_STENCIL_WRITE_MASK,
		D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_STENCIL_OP_KEEP, D3D11_COMPARISON_ALWAYS
	};

	if (FAILED(device->CreateDepthStencilState(&desc, &dss))) return false;

	return true;
}

bool DXGraphicAPI::CDxGraphic::CreateStencilBuffer(int w, int h)
{

	D3D11_TEXTURE2D_DESC texdesc =
	{
		static_cast<UINT>(w), static_cast<UINT>(h), 1, 1,
		DXGI_FORMAT_R24G8_TYPELESS, sampledesc, D3D11_USAGE_DEFAULT, D3D11_BIND_DEPTH_STENCIL | D3D11_BIND_SHADER_RESOURCE
	};

	if (FAILED(device->CreateTexture2D(&texdesc, nullptr, &depthtex))) return false;


	D3D11_DEPTH_STENCIL_VIEW_DESC dsvdesc =
	{
		DXGI_FORMAT_D24_UNORM_S8_UINT, D3D11_DSV_DIMENSION_TEXTURE2D
	};

	if (FAILED(device->CreateDepthStencilView(depthtex, &dsvdesc, &dsv))) return false;

	return true;
}

bool DXGraphicAPI::CDxGraphic::CreateShaderFromCompiledFiles()
{
	auto WideStr2MultiByte = [](const std::wstring wstr) -> std::string
	{
		size_t size = ::WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, nullptr, 0, nullptr, nullptr);
		std::vector<char> buf;
		buf.resize(size);
		::WideCharToMultiByte(CP_OEMCP, 0, wstr.c_str(), -1, &buf.front(), static_cast<int>(size), nullptr, nullptr);
		std::string ret(&buf.front(), buf.size() - 1);
		return ret;
	};

	std::wstring filepath;
	filepath.resize(MAX_PATH);
	::GetModuleFileName(g_hInstance, &filepath.front(), MAX_PATH);
	::PathRemoveFileSpec(&filepath.front());

	// vertex shader
	std::string csofile = WideStr2MultiByte(filepath);
	csofile.append("\\VertexShader.cso");
	std::ifstream ifs(csofile, std::ios::in | std::ios::binary);
	if (ifs.fail()) return false;
	ifs.seekg(0, std::ifstream::end);
	size_t csosize = static_cast<size_t>(ifs.tellg());
	ifs.seekg(0, std::ifstream::beg);
	std::vector<char> csodata;
	csodata.resize(csosize);
	ifs.read(&csodata.front(), csosize);

	if (FAILED(device->CreateVertexShader(&csodata.front(), csosize, nullptr, &vertexshader.p)))
		return false;

	// 入力するデータのレイアウトを定義
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT , 0, sizeof(float) * 3, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	UINT num = ARRAYSIZE(layout);

	if (FAILED(device->CreateInputLayout(layout, num, &csodata.front(), csosize, &inputlayout.p)))
		return false;

	// geometry shader
	ifs.close();
	csodata.clear();
	csofile = WideStr2MultiByte(filepath);
	csofile.append("\\GeometryShader.cso");
	ifs.open(csofile, std::ios::in | std::ios::binary);
	if (ifs.fail()) return false;
	ifs.seekg(0, std::ifstream::end);
	csosize = static_cast<size_t>(ifs.tellg());
	ifs.seekg(0, std::ifstream::beg);
	csodata.resize(csosize);
	ifs.read(&csodata.front(), csosize);

	if (FAILED(device->CreateGeometryShader(&csodata.front(), csosize, nullptr, &geometryshader.p)))
		return false;

	// pixel shader
	ifs.close();
	csofile = WideStr2MultiByte(filepath);
	csofile.append("\\PixelShader.cso");
	ifs.open(csofile, std::ios::in | std::ios::binary);
	if (ifs.fail())
		return false;
	ifs.seekg(0, std::ifstream::end);
	csosize = static_cast<size_t>(ifs.tellg());
	ifs.seekg(0, std::ifstream::beg);
	csodata.clear();
	csodata.resize(csosize);
	ifs.read(&csodata.front(), csosize);

	if (FAILED(device->CreatePixelShader(&csodata.front(), csosize, nullptr, &pixelshader.p)))
		return false;

	return true;
}

bool DXGraphicAPI::CDxGraphic::CreateConstantBuffer()
{
	// TODO : Create CB
	D3D11_BUFFER_DESC matrixdesc =
	{
		sizeof(MatrixBuffer),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_CONSTANT_BUFFER
	};

	if (FAILED(device->CreateBuffer(&matrixdesc, nullptr, &matrixbuffer)))
		return false;

	return true;
}

void DXGraphicAPI::CDxGraphic::ReleaseComPtr()
{
	pixelshader.Release();
	geometryshader.Release();
	vertexshader.Release();
	inputlayout.Release();

	rs.Release();
	dss.Release();
	dsv.Release();
	depthtex.Release();

	rtv.Release();
	backbuffer.Release();

	swapchain.Release();
	context.Release();
	device.Release();
}

void DXGraphicAPI::CDxGraphic::SetWindowHandle(HWND hWnd)
{
	m_WindowHandle = hWnd;
}

bool DXGraphicAPI::CDxGraphic::InitD3D(int w, int h)
{
	if (w == 0 || h == 0)
		return false;

	if (!CreateDeviceAndSwapChain(w, h))
	{
		ReleaseComPtr();
		return false;
	}

	if (!CreateRenderTarget())
	{
		ReleaseComPtr();
		return false;
	}

	if (!CreateDefaultRasterizerState())
	{
		ReleaseComPtr();
		return false;
	}

	if (!CreateDepthStencilState())
	{
		ReleaseComPtr();
		return false;
	}

	if (!CreateStencilBuffer(w, h))
	{
		ReleaseComPtr();
		return false;
	}

	// レンダーターゲットに深度/ステンシルテクスチャを設定
	context->OMSetRenderTargets(1, &rtv.p, dsv);
	// ビューポートの設定
	D3D11_VIEWPORT vp[] =
	{
		{ 0, 0, static_cast<FLOAT>(w), static_cast<FLOAT>(h), 0, 1.0f }
	};
	context->RSSetViewports(1, vp);

	if (!CreateShaderFromCompiledFiles())
	{
		ReleaseComPtr();
		return false;
	}

	if (!CreateConstantBuffer())
	{
		ReleaseComPtr();
		return false;
	}

	return true;
}

void DXGraphicAPI::CDxGraphic::Render()
{
	UINT strides = sizeof(CoordColor);
	UINT offset = 0;

	if (context == nullptr) return;

	// バックバッファと深度バッファのクリア
	FLOAT backcolor[4] = { 1.f, 1.f, 1.f, 1.f };
	context->ClearRenderTargetView(rtv, backcolor);
	context->ClearDepthStencilView(dsv, D3D11_CLEAR_DEPTH, 1.0f, 0);

	// 頂点データに渡すデータのレイアウトを設定
	context->IASetInputLayout(inputlayout);

	// 頂点シェーダー, ジオメトリシェーダー, ピクセルシェーダーの設定
	context->VSSetShader(vertexshader, nullptr, 0);
	context->GSSetShader(geometryshader, nullptr, 0);
	context->PSSetShader(pixelshader, nullptr, 0);

	// ラスタライザーステートを設定
	context->RSSetState(rs);

	MatrixBuffer matrixbuf = {
		// シェーダーでは列優先(column_major)で行列データを保持するため, 転置を行う
		DirectX::XMMatrixTranspose(d3dprojmatrix),
		DirectX::XMMatrixTranspose(d3dviewmatrix),
		DirectX::XMMatrixTranspose(d3dworldmatrix)
	};

	// マトリックスバッファの設定
	context->UpdateSubresource(matrixbuffer, 0, nullptr, &matrixbuf, 0, 0);
	context->VSSetConstantBuffers(0, 1, &matrixbuffer.p);
	context->GSSetConstantBuffers(0, 1, &matrixbuffer.p);


	// 深度・ステンシルバッファの使用方法を設定
	context->OMSetDepthStencilState(dss, 0);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->IASetVertexBuffers(0, 1, &vertexbuffer.p, &strides, &offset);
	context->IASetIndexBuffer(indexbuffer, DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(numindices, 0, 0);

	// 作成したプリミティブをウィンドウへ描画
	if (swapchain != nullptr)
		swapchain->Present(0, 0);
}

void DXGraphicAPI::CDxGraphic::LoadSampleData(int w, int h)
{
	float nearz = 1 / 1000.0f;
	float farz = 10.0f;

	d3dprojmatrix = DirectX::XMMatrixPerspectiveFovRH(M_PI / 4.0f, 1.0f * w / h, nearz, farz);

	m_cameraposition = Math::Vector3(3.5f, 3.5f, 3.5f);
	m_lookatpoint = Math::Vector3(0.0f, 0.0f, 0.0f);

	float upsetz = m_camupset ? -1.0f : 1.0f;

	DirectX::XMVECTOR eye = DirectX::XMVectorSet(m_cameraposition.x, m_cameraposition.y, m_cameraposition.z, 0.0f);
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(m_lookatpoint.x, m_lookatpoint.y, m_lookatpoint.z, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 0.0f, upsetz, 0.0f);

	d3dviewmatrix = DirectX::XMMatrixLookAtRH(eye, focus, up);

	std::vector<float> vertexarray;
	for (Vertex v : InputData)
	{
		vertexarray.push_back(v.position[0]);
		vertexarray.push_back(v.position[1]);
		vertexarray.push_back(v.position[2]);

		vertexarray.push_back(v.color[0]);
		vertexarray.push_back(v.color[1]);
		vertexarray.push_back(v.color[2]);
		vertexarray.push_back(v.color[3]);
	}


	std::vector<int> indexarray;
	for (Triangle tri : InputTriangle)
	{
		indexarray.push_back(tri.indices[0]);
		indexarray.push_back(tri.indices[1]);
		indexarray.push_back(tri.indices[2]);
	}
	numindices = static_cast<UINT>(indexarray.size());

	vertexbuffer.Release();
	D3D11_BUFFER_DESC bdvertex =
	{
		static_cast<UINT>(sizeof(float) * vertexarray.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_VERTEX_BUFFER
	};
	D3D11_SUBRESOURCE_DATA srdv = { &vertexarray.front() };
	device->CreateBuffer(&bdvertex, &srdv, &vertexbuffer.p);

	indexbuffer.Release();
	D3D11_BUFFER_DESC bdindex =
	{
		static_cast<UINT>(sizeof(int) * indexarray.size()),
		D3D11_USAGE_DEFAULT,
		D3D11_BIND_INDEX_BUFFER
	};
	D3D11_SUBRESOURCE_DATA srdind = { &indexarray.front() };
	device->CreateBuffer(&bdindex, &srdind, &indexbuffer.p);

	Render();
}

void DXGraphicAPI::CDxGraphic::UpdateMatrices(int w, int h)
{
	if (w == 0 || h == 0) return;

	float nearz = 1 / 1000.0f;
	float farz = 10.0f;

	d3dprojmatrix = DirectX::XMMatrixPerspectiveFovRH(static_cast<float>((M_PI / 4)), 1.0f * w / h, nearz, farz);
}

bool DXGraphicAPI::CDxGraphic::ResizeView(int w, int h)
{
	if (w == 0 || h == 0 || device == nullptr)
		return false;


	ID3D11RenderTargetView* irtv = nullptr;
	context->OMSetRenderTargets(1, &irtv, nullptr);
	rtv.Release();
	backbuffer.Release();

	dsv.Release();
	depthtex.Release();

	if (FAILED(swapchain->ResizeBuffers(swapchaincount, w, h, swapchainformat, 0)))
		return false;

	if (!CreateRenderTarget())
	{
		ReleaseComPtr();
		return false;
	}

	if (!CreateStencilBuffer(w, h))
	{
		ReleaseComPtr();
		return false;
	}

	// レンダーターゲットに深度/ステンシルテクスチャを設定
	context->OMSetRenderTargets(1, &rtv.p, dsv);
	// ビューポートの設定
	D3D11_VIEWPORT vp[] =
	{
		{ 0, 0, static_cast<FLOAT>(w), static_cast<FLOAT>(h), 0, 1.0f }
	};
	context->RSSetViewports(1, vp);

	return true;
}

void DXGraphicAPI::CDxGraphic::CameraRotateZ(float delta)
{
	DirectX::XMMATRIX rotZ = DirectX::XMMatrixRotationZ(delta);
	DirectX::XMVECTOR eye = DirectX::XMVectorSet(m_cameraposition.x, m_cameraposition.y, m_cameraposition.z, 0.0f);

	eye = DirectX::XMVector3Transform(eye, rotZ);
	m_cameraposition = Math::Vector3(DirectX::XMVectorGetX(eye), DirectX::XMVectorGetY(eye), DirectX::XMVectorGetZ(eye));
	float upsetz = m_camupset ? -1.0f : 1.0f;
	DirectX::XMVECTOR focus = DirectX::XMVectorSet(m_lookatpoint.x, m_lookatpoint.y, m_lookatpoint.z, 0.0f);
	DirectX::XMVECTOR up = DirectX::XMVectorSet(0.0f, 0.0f, upsetz, 0.0f);

	d3dviewmatrix = DirectX::XMMatrixLookAtRH(eye, focus, up);
}
