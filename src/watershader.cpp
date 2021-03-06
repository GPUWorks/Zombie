#include "watershader.h"

namespace zombie {

	WaterShader::WaterShader() {
		aVerIndex_ = -1;
		aTexIndex_ = -1;

		// Collect the uniforms indexes.
		uProjIndex_ = -1;
		uModelIndex_ = -1;
		uGlobalCenterPosIndex_ = -1;
		uTime_ = -1;
	}

	WaterShader::WaterShader(std::string vShaderFile, std::string fShaderFile) {
		shader_.bindAttribute("aVer");
		shader_.loadAndLinkFromFile(vShaderFile, fShaderFile);
		
		shader_.glUseProgram();

		// Collect the vertex buffer attributes indexes.
		aVerIndex_ = shader_.getAttributeLocation("aVer");

		// Collect the vertex buffer uniforms indexes.
		uProjIndex_ = shader_.getUniformLocation("uProj");
		uModelIndex_ = shader_.getUniformLocation("uModel");
		uGlobalCenterPosIndex_ = shader_.getUniformLocation("uGlobalCenterPos");
		uTime_ = shader_.getUniformLocation("uTime");
	}

	void WaterShader::glUseProgram() const {
		shader_.glUseProgram();
	}

	// Vertex buffer Attributes. ---------------------------

	void WaterShader::setGlVer2dCoordsA(const GLvoid* data) const {
		mw::glVertexAttribPointer(aVerIndex_, 2, GL_FLOAT, GL_FALSE, 0, data);
		mw::glEnableVertexAttribArray(aVerIndex_);
	}

	void WaterShader::setGlVer2dCoordsA(GLsizei stride, const GLvoid* data) const {
		mw::glVertexAttribPointer(aVerIndex_, 2, GL_FLOAT, GL_FALSE, stride, data);
		mw::glEnableVertexAttribArray(aVerIndex_);
	}

	// Uniforms. -------------------------------------------

	void WaterShader::setGlProjectionMatrixU(const mw::Matrix44& matrix) const {
		mw::glUniformMatrix4fv(uProjIndex_, 1, false, matrix.data());
	}

	void WaterShader::setGlModelMatrixU(const mw::Matrix44& matrix) const {
		mw::glUniformMatrix4fv(uModelIndex_, 1, false, matrix.data());
	}

	void WaterShader::setGlGlobalCenterPositionU(Position position) const {
		mw::glUniform2f(uGlobalCenterPosIndex_, position.x, position.y);
	}
	
	void WaterShader::setGlColorU(float red, float green, float blue, float alpha) const {
		mw::glUniform4f(uGlobalCenterPosIndex_, red, green, blue, alpha);
	}

	void WaterShader::setGlColorU(const mw::Color& color) const {
		mw::glUniform4f(uGlobalCenterPosIndex_, color.red_, color.green_, color.blue_, color.alpha_);
	}

	void WaterShader::setGlTimeU(float time) const {
		mw::glUniform1f(uTime_, time);
	}

} // Namespace zombie.
