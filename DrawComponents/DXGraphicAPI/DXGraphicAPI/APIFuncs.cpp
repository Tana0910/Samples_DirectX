#pragma once
#include "pch.h"
#include "APIFuncs.h"
#include "DxGraphic.h"

DXGraphicAPI::CDxGraphic g_dxgra;

void SetWindowHandle(HWND hWnd)
{
	g_dxgra.SetWindowHandle(hWnd);
}

void DrawSampleData(int w, int h)
{
	if (g_dxgra.InitD3D(w, h))
	{
		g_dxgra.LoadSampleData(w, h);
	}
}

bool ResizeView(int w, int h)
{
	if (g_dxgra.ResizeView(w, h))
	{
		g_dxgra.UpdateMatrices(w, h);
		g_dxgra.Render();
		return true;
	}
	return false;
}
