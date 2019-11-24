#pragma once

typedef void* (__cdecl *IWEBSIMTEXTURE)(); 

class IWebSimTextureCallback
{
public:
	// version 1.4
	virtual void OnTextureSpawned(char* szTexture) {};
};


class IWebSimTexture
{
public:
	// version 1.4
	virtual long MapTexture(char* szTexture, int width, int height, bool bHasAlpha, float alphaLevel, char* szTextureFile, bool bUpsideDown = false, void* m_handler = NULL) { return 0; };
	virtual long UnmapTexture(char* szTexture) { return 0; }; // be carefull do not remove predefined textures even if you overwrite it.
	virtual void SetTransparencyLevel(char* szTexture, float level) {};
    virtual void Repaint(char* szTexture) {};

	virtual void RegisterCallback(IWebSimTextureCallback* pIWebSimTextureCallback) {};
	
	virtual long AddRegion ( char* szTexture, char* szID, int left, int top, int width, int height, char *szURL, int FPS = 30) { return 0; };

	virtual void Shutdown() {};
};
