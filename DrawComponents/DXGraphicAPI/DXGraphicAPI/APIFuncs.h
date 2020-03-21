#pragma once
#ifndef DLL_EXPORTS
#define DLLAPI extern "C" __declspec(dllexport)
#else
#define DLLAPI extern "C" __declspec(dllimport)
#endif // !DLL_EXPORTS


DLLAPI void SetWindowHandle(HWND hWnd);

DLLAPI void DrawSampleData(int w, int h);

DLLAPI bool ResizeView(int w, int h);

DLLAPI void RotateZ(float delta);
