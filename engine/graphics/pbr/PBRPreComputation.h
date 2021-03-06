#pragma once

#include "graphics/assets/Shader.h"
#include "graphics/assets/Texture.h"
#include "graphics/assets/Cubemap.h"
#include "utils/Math.h"

// Much of this class seems like a lot of code duplication (ignoring existing abstractions)
// but rendering to a cubemap and the specific utilization of the framebuffer makes it
// probably not worth abstracting more, at least not right now. Might get system working with
// existing cubemaps.

class PBRPreComputation {
public:
	PBRPreComputation(const std::string& envName = "newport_loft");
	~PBRPreComputation();

	void SetHDR(Texture2D* hdrTexture);
	Texture2D* GetHDR();

	inline Cubemap* GetIrradianceMap() { return mIrradianceMap; }
	inline Cubemap* GetPrefilterMap() { return mPrefilterMap; }
	inline Cubemap* GetEnvironmentMap() { return mEnvironmentMap; }
	inline Texture2D* GetBRDF() { return mBRDFTexture; }

	void RenderBRDF();

private:
	// Utility functions - replace these with mesh prefabs later
	void CreateCube();
	void CreateQuad();
	void InitializeShaders();

private:
	Shader* mHdrToCubemap;
	Shader* mConvoluteIrradiance;
	Shader* mPrefilter;
	Shader* mBRDF;

	Texture2D* mHdrTexture;
	Texture2D* mBRDFTexture;
	Cubemap* mEnvironmentMap;
	Cubemap* mIrradianceMap;
	Cubemap* mPrefilterMap;

	unsigned int mCaptureFbo;
	unsigned int mCaptureRbo;

	// Utility handles - replace these with mesh prefabs later
	GLuint mCubeVao, mCubeVbo;
	GLuint mQuadVao, mQuadVbo;
};