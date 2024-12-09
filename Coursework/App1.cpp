// Lab1.cpp
// Lab 1 example, simple coloured triangle mesh
#include "App1.h"

App1::App1()
{
	sea = nullptr;
	sand = nullptr;
}

void App1::init(HINSTANCE hinstance, HWND hwnd, int screenWidth, int screenHeight, Input* in, bool VSYNC, bool FULL_SCREEN)
{
	// Call super/parent init function (required!)
	BaseApplication::init(hinstance, hwnd, screenWidth, screenHeight, in, VSYNC, FULL_SCREEN);
	camera->setPosition(0.0f, 10.0f, 0.0f);
	// Create Mesh object and shader object
	sea = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture(L"sea", L"res/sea.jpg");
	manipulationShader = new ManipulationShader(renderer->getDevice(), hwnd);
	lightShader = new LightShader(renderer->getDevice(), hwnd);

	orthoMesh = new OrthoMesh(renderer->getDevice(), renderer->getDeviceContext(), screenWidth, screenHeight);

	sand = new PlaneMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture(L"sand", L"res/sand.jpg");
	textureMgr->loadTexture(L"beach_heightmap", L"res/beach_heightmap.png");
	heightmapShader = new HeightmapShader(renderer->getDevice(), hwnd);

	bunny = new CubeMesh(renderer->getDevice(), renderer->getDeviceContext());
	textureMgr->loadTexture(L"bunny", L"res/bunny.png");

	depthShader = new DepthShader(renderer->getDevice(), hwnd);
	shadowShader = new ShadowShader(renderer->getDevice(), hwnd);

	hBlurShader = new HorizontalBlurShader(renderer->getDevice(), hwnd);
	vBlurShader = new VerticalBlurShader(renderer->getDevice(), hwnd);

	hBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);
	vBlurTexture = new RenderTexture(renderer->getDevice(), screenWidth, screenHeight, SCREEN_NEAR, SCREEN_DEPTH);

	// shadow map 
	int shadowmapWidth = 1024;
	int shadowmapHeight = 1024;
	int sceneWidth = 100;
	int sceneHeight = 50;
	for (int i = 0; i < 2; i++) {
		shadowMap[i] = new ShadowMap(renderer->getDevice(), shadowmapWidth, shadowmapHeight);
	}
	light[0] = new Light();
	light[0]->setAmbientColour(0.3f, 0.3f, 0.3f, 1.0f);
	light[0]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light[0]->setDirection(0.0f, -1.0f, 0.0f);
	light[0]->setPosition(-20.f, 10.f, -40.f);
	light[0]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);

	light[1] = new Light();
	light[1]->setAmbientColour(1.0f, 0.0f, 0.0f, 1.0f);
	light[1]->setDiffuseColour(1.0f, 1.0f, 1.0f, 1.0f);
	light[1]->setDirection(0.0f, -0.7f, 0.7f);
	light[1]->setPosition(0.f, -1.0f, -1.0f);
	light[1]->generateOrthoMatrix((float)sceneWidth, (float)sceneHeight, 0.1f, 100.f);
}


App1::~App1()
{
	// Run base application deconstructor
	BaseApplication::~BaseApplication();

	// Release the Direct3D object.
	if (sea)
	{
		delete sea;
		sea = 0;
	}

	if (sand)
	{
		delete sand;
		sand = 0;
	}
	if (bunny) {
		delete bunny;
		bunny = 0;
	}
}


bool App1::frame()
{
	bool result;

	result = BaseApplication::frame();
	if (!result)
	{
		return false;
	}

	// Render the graphics.
	result = render();
	if (!result)
	{
		return false;
	}

	return true;
}

bool App1::render()
{
	// Clear the scene. (default blue colour)
	renderer->beginScene(0.39f, 0.58f, 0.92f, 1.0f);
	time += timer->getTime();
	// Generate the view matrix based on the camera's position.
	camera->update();


	firstPass();
	depthPass();
	finalPass();
	// Render GUI


	return true;
}

void App1::firstPass() {

	shadowMap[0]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());
	// get the world, view, and projection matrices from the camera and d3d objects.


	light[0]->generateViewMatrix();
	XMMATRIX lightViewMatrix = light[0]->getViewMatrix();
	XMMATRIX lightProjectionMatrix = light[0]->getOrthoMatrix();
	XMMATRIX worldMatrix = renderer->getWorldMatrix();

	sea->sendData(renderer->getDeviceContext()); // handle geometry 
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sea->getIndexCount());


	worldMatrix *= XMMatrixTranslation(0.0, 0.0, -100.0);
	sand->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sand->getIndexCount());

	worldMatrix = renderer->getWorldMatrix() * XMMatrixTranslation(50.0f, 5.0f, -50.0f);
	XMMATRIX ScalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, ScalingMatrix);
	bunny->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), bunny->getIndexCount());

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::depthPass() {
	shadowMap[1]->BindDsvAndSetNullRenderTarget(renderer->getDeviceContext());

	// get the world, view, and projection matrices from the camera and d3d objects.


		light[1]->generateViewMatrix();
		XMMATRIX lightViewMatrix = light[1]->getViewMatrix();
		XMMATRIX lightProjectionMatrix = light[1]->getOrthoMatrix();
		XMMATRIX worldMatrix = renderer->getWorldMatrix();

	sea->sendData(renderer->getDeviceContext()); // handle geometry 
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sea->getIndexCount());


	worldMatrix *= XMMatrixTranslation(0.0, 0.0, -100.0);
	sand->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), sand->getIndexCount());

	worldMatrix = renderer->getWorldMatrix() * XMMatrixTranslation(50.0f, 5.0f, -50.0f);
	XMMATRIX ScalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, ScalingMatrix);
	bunny->sendData(renderer->getDeviceContext());
	depthShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, lightViewMatrix, lightProjectionMatrix);
	depthShader->render(renderer->getDeviceContext(), bunny->getIndexCount());

	renderer->setBackBufferRenderTarget();
	renderer->resetViewport();
}

void App1::horizontalBlur()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeX = (float)hBlurTexture->getTextureWidth();
	hBlurTexture->setRenderTarget(renderer->getDeviceContext());
	hBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 1.0f, 1.0f, 0.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	orthoMatrix = hBlurTexture->getOrthoMatrix();

	// Render for Horizontal Blur
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	hBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, renderTexture->getShaderResourceView(), screenSizeX);
	hBlurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}

void App1::verticalBlur()
{
	XMMATRIX worldMatrix, baseViewMatrix, orthoMatrix;

	float screenSizeY = (float)vBlurTexture->getTextureHeight();
	vBlurTexture->setRenderTarget(renderer->getDeviceContext());
	vBlurTexture->clearRenderTarget(renderer->getDeviceContext(), 0.0f, 1.0f, 1.0f, 1.0f);

	worldMatrix = renderer->getWorldMatrix();
	baseViewMatrix = camera->getOrthoViewMatrix();
	// Get the ortho matrix from the render to texture since texture has different dimensions being that it is smaller.
	orthoMatrix = vBlurTexture->getOrthoMatrix();

	// Render for Vertical Blur
	renderer->setZBuffer(false);
	orthoMesh->sendData(renderer->getDeviceContext());
	vBlurShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, baseViewMatrix, orthoMatrix, hBlurTexture->getShaderResourceView(), screenSizeY);
	vBlurShader->render(renderer->getDeviceContext(), orthoMesh->getIndexCount());
	renderer->setZBuffer(true);

	// Reset the render target back to the original back buffer and not the render to texture anymore.
	renderer->setBackBufferRenderTarget();
}


void App1::finalPass() {
	// get the world, view, projection, and ortho matrices from the camera and Direct3D objects.
	XMMATRIX worldMatrix = renderer->getWorldMatrix();
	XMMATRIX viewMatrix = camera->getViewMatrix();
	XMMATRIX projectionMatrix = renderer->getProjectionMatrix();


	// Render floor
	sea->sendData(renderer->getDeviceContext()); // handle vertex manipulation 
	manipulationShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix, textureMgr->getTexture(L"sea"), shadowMap[0]->getDepthMapSRV(), time, amplitude, speed, frequency);
	manipulationShader->render(renderer->getDeviceContext(), sea->getIndexCount());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"sea"), shadowMapGetter[0], light[0]);
	shadowShader->render(renderer->getDeviceContext(), bunny->getIndexCount());

	worldMatrix *= XMMatrixTranslation(0.0, 0.0, -100.0);
	sand->sendData(renderer->getDeviceContext());
	heightmapShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"sand"), textureMgr->getTexture(L"beach_heightmap"), 3.0f);
	heightmapShader->render(renderer->getDeviceContext(), sand->getIndexCount());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"sea"), shadowMapGetter[0], light[0]);
	shadowShader->render(renderer->getDeviceContext(), bunny->getIndexCount());


	worldMatrix = renderer->getWorldMatrix() * XMMatrixTranslation(50.0f, 5.0f, -50.0f);
	XMMATRIX ScalingMatrix = XMMatrixScaling(1.0f, 1.0f, 1.0f);
	worldMatrix = XMMatrixMultiply(worldMatrix, ScalingMatrix);
	bunny->sendData(renderer->getDeviceContext());
	shadowShader->setShaderParameters(renderer->getDeviceContext(), worldMatrix, viewMatrix, projectionMatrix,
		textureMgr->getTexture(L"bunny"), shadowMapGetter[0], light[0]);
	shadowShader->render(renderer->getDeviceContext(), bunny->getIndexCount());


	gui();

	// Present the rendered scene to the screen.
	renderer->endScene();
}

void App1::gui()
{
	// Force turn off unnecessary shader stages.
	renderer->getDeviceContext()->GSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->HSSetShader(NULL, NULL, 0);
	renderer->getDeviceContext()->DSSetShader(NULL, NULL, 0);

	// Build UI
	ImGui::Text("FPS: %.2f", timer->getFPS());
	ImGui::Checkbox("Wireframe mode", &wireframeToggle);
	ImGui::SliderFloat("Amplitude", &amplitude, 0.1f, 0.25f);
	ImGui::SliderFloat("Frequency", &frequency, 0.1f, 1.0f);
	ImGui::SliderFloat("Speed", &speed, 1.0f, 5.0f);

	// Render UI
	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}