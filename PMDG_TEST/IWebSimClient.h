#pragma once

typedef void* (__cdecl *IWEBSIMCLIENT)(int, char*); 

class IWebSimCallback
{
public:
	//version 1.3
	virtual void OnPageLoaded(char* szURL, int httpStatusCode) {};
	virtual void OnJavaScriptFunction(char* szArgument) {};
	virtual bool OnBeforeBrowse(char* szURL, bool bRedirect) { return false; };
	virtual void OnBrowserClose() {};

	// version 2.1
#if defined(WEBSIMSOCKET)
	virtual void OnWebSimSocketOpen() {};
	virtual void OnWebSimSocketClose() {};
#endif

};

class IWebSimClient
{
public:
	// version 1.2
	virtual void GoTo(char* szURL, char* szFrame = NULL) {};
	virtual void LoadString(char* szHTML, char* szURL, char* szFrame = NULL) {};
	virtual void ExecuteJavaScript(char* szScript, char* szFrame = NULL) {};
	virtual void Back() {};
	virtual void Forward() {};
	virtual void Stop() {};
	virtual void Reload() {};
	virtual void Show(bool bShow) {};
	virtual void Move(int x, int y, int width, int height) {} // FSX only
	virtual void SetFocus(bool bSet) {}; // X-plane only

	// version 1.3	
	virtual void RegisterCallback(IWebSimCallback* pWebSimCallback) {};
	virtual void CrossDomainAllow(char* szFrom, char* szTo) {};
	virtual void FreezeRendering(bool bFreeze, bool bClear) {}; // keeps old screen shot when freezed
	virtual bool HasBrowser() { return false; };
	virtual bool IsDebugEnabled() {return false;};

	// version 1.4
	virtual void CloseBrowser() {};  // experimental
	
	// version 1.5
	virtual char* GetURL(char* szFrame = NULL) { return NULL; };
	virtual void EnableLight(bool bLight) {}; // X-plane only
	virtual void SetColorFilter(float red, float green, float blue, float alpha) {}; // X-plane only

	// version 2.1
	virtual void UnregisterCallback(IWebSimCallback* pWebSimCallback) {};

	// reserved
	virtual void SetZoom(float) {};
	virtual void SetOffset(int,int) {};
	virtual void GetOffset(int*,int*) {};

};
