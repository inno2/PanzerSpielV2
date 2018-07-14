#pragma once
#include <windows.h>
#include "Direct3D11.h"
#include "Color.h"
#include "Eigen/Dense"


using namespace Eigen;

struct Vertex
{
	float X, Y, Z;
};

class DX11Renderer
{
public:
	DX11Renderer();
	void BeginScene(Color background);
	void EndScene();
	~DX11Renderer();

	bool Initialize(HWND window, unsigned int screenWidth, unsigned int screenHeight, bool enable_vsync, bool fullscreen, float nearplane, float farplane);
	void MakeProjectionMatrix(float aspect, float fov, float near_z, float far_z, bool leftHanded, Matrix4f & ppm);
	void Shutdown();
	ID3D11Device* GetDevice();
	ID3D11DeviceContext* GetDeviceContext();
	void GetWorldMatrix(Matrix4f& world);
	void GetProjectionMatrix(Matrix4f& projection);

private:
	bool vsync_enabled;

	size_t m_videoCardMemory;
	char m_videoCardDescription[128];

	IDXGISwapChain* m_swapChain;
	ID3D11Device* m_device;                     
	ID3D11DeviceContext* m_deviceContext;          
	ID3D11RenderTargetView* m_backbuffer;    
	HWND m_hWindow;
	ID3D11Texture2D* m_depthStencilBuffer;
	ID3D11DepthStencilState* m_depthStencilState;
	ID3D11DepthStencilView* m_depthStencilView;
	ID3D11RasterizerState* m_rasterState;
	Matrix4f m_projectionMatrix;
	Matrix4f m_worldMatrix;
	Matrix4f m_orthoMatrix;
};