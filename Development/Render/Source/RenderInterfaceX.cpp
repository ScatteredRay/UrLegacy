 
#include "RenderInterface.h" 

#if USING_DX
#include "Core.h"
#include "UTypes.h"
#include "UMath.h"

IDirect3D9* gD3D;

HRenderDevice RICreateContext(HWindowContext WC)
{

	D3DPRESENT_PARAMETERS D3DParams;
	D3DParams.Windowed = true;
	D3DParams.BackBufferWidth = 800;
	D3DParams.BackBufferHeight = 600;
	D3DParams.MultiSampleType = D3DMULTISAMPLE_NONE;
	D3DParams.MultiSampleQuality = 0;
	D3DParams.SwapEffect = D3DSWAPEFFECT_DISCARD;
	D3DParams.Flags = 0;
	D3DParams.FullScreen_RefreshRateInHz = D3DPRESENT_RATE_DEFAULT;
	D3DParams.PresentationInterval = D3DPRESENT_INTERVAL_DEFAULT;
	D3DParams.BackBufferFormat = D3DFMT_UNKNOWN;
	D3DParams.BackBufferCount = 3;
	D3DParams.hDeviceWindow = WC.hWnd;
	D3DParams.EnableAutoDepthStencil = true;
	D3DParams.AutoDepthStencilFormat = D3DFMT_D16;

	IDirect3DDevice9* pDevice;

	assert(
		SUCCEEDED(gD3D->CreateDevice(	D3DADAPTER_DEFAULT,
										D3DDEVTYPE_HAL,
										WC.hWnd,
										D3DCREATE_SOFTWARE_VERTEXPROCESSING,
										&D3DParams,
										&pDevice))
										);
	return HRenderDevice(pDevice);
}

bool RIDestroyDevice(HRenderDevice Dev)
{
	Dev.Device->Release();
	return true;
}

void RIBeginScene(HRenderDevice Dev)
{
	Dev.Device->BeginScene();
}

void RIEndScene(HRenderDevice Dev)
{
	Dev.Device->EndScene();
}

void RIPresent(HRenderDevice Dev)
{
	Dev.Device->Present(NULL, NULL, NULL, NULL);
}

//TODO: break the optionals into seperate functions so we don't bother all the run time checking.
void RIClear(HRenderDevice Dev, uint ClearFlags, KColor& Clear, float Depth, int Stencil, KColor& Accum)
{
	Dev.Device->Clear(0, NULL, ClearFlags, Clear.AsInt(), Depth, Stencil);
}

HVertexBuffer RICreateVertexBuffer(HRenderDevice Dev, size_t Size)
{
	IDirect3DVertexBuffer9* pVBO;
	Dev.Device->CreateVertexBuffer(Size, 0, D3DFVF_XYZ, D3DPOOL_DEFAULT, &pVBO, NULL);
	return HVertexBuffer(pVBO);
}

void RIDeleteVertexBuffer(HRenderDevice Dev, HVertexBuffer VBO)
{
	VBO.VBO->Release();
}

void RISetBufferData(HRenderDevice Dev, HVertexBuffer VBO, void* Data, size_t Size)
{
	void* Dest;
	VBO.VBO->Lock(0, Size, &Dest, D3DLOCK_DISCARD);
	appMemcpy(Dest, Data, Size);
	VBO.VBO->Unlock();
}

void RIBindBuffer(HRenderDevice Dev, HVertexBuffer VBO, uint32 StreamNumber)
{
	Dev.Device->SetStreamSource(StreamNumber, VBO.VBO, 0, 3*4);
}

void RIDrawPrimitive(HRenderDevice Dev, uint DrawType, uint StartVertex, uint PrimitiveCount)
{
	Dev.Device->SetFVF(D3DFVF_XYZ);
	Dev.Device->DrawPrimitive((D3DPRIMITIVETYPE)DrawType, StartVertex, PrimitiveCount);
}

void RISetColor(HRenderDevice Dev, KColor& Color)
{
}

void RISetMatrix(HRenderDevice Dev, uint TransformState, Matrix4& Transform)
{
	Dev.Device->SetTransform((D3DTRANSFORMSTATETYPE)TransformState, (D3DMATRIX*)&Transform);
}

#endif //USING_DX