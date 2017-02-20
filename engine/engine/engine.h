#pragma once

#include "../graphics/window.h"

class Engine
{
public:
	Engine() : mQuit(false) {}
	~Engine() {}

	void Start(int width = 640, int height = 480, const char* title = "");
	void Quit();

	void SetTitle(const char* title);
	void SetDimensions(int width, int height);

	int GetWidth() { return mWindow->GetWidth(); }
	int GetHeight() { return mWindow->GetHeight(); }

	void SetClearColor(Color color);

protected:
	virtual void Init() = 0;
	virtual void Cleanup() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;

private:
	void Loop();
	void InitBackend(int width, int height, const char* title);
	void CleanupBackend();

private:
	Window* mWindow;
	bool mQuit;
	int mWidth, mHeight;
};