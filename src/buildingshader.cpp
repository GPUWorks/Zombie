#include "buildingshader.h"

namespace zombie {

	BuildingShader::BuildingShader() {
		aVerIndex_ = -1;
		aTexIndex_ = -1;

		// Collect the vertex buffer uniforms indexes.
		uProjIndex_ = -1;
		uModelIndex_ = -1;
		uGlobalCenterPosIndex_ = -1;
		uGlobalHumanPosIndex_ = -1;
	}

	BuildingShader::BuildingShader(std::string vShaderFile, std::string fShaderFile) {
		shader_.bindAttribute("aVer");
		shader_.bindAttribute("aTex");
		shader_.loadAndLinkFromFile(vShaderFile, fShaderFile);
		
		shader_.glUseProgram();

		// Collect the vertex buffer attributes indexes.
		aVerIndex_ = shader_.getAttributeLocation("aVer");
		aTexIndex_ = shader_.getAttributeLocation("aTex");

		// Collect the vertex buffer uniforms indexes.
		uProjIndex_ = shader_.getUniformLocation("uProj");
		uModelIndex_ = shader_.getUniformLocation("uModel");
		uGlobalCenterPosIndex_ = shader_.getUniformLocation("uGlobalCenterPos");
		uGlobalHumanPosIndex_ = shader_.getUniformLocation("uGlobalHumanPos");
	}

	void BuildingShader::glUseProgram() const {
		shader_.glUseProgram();
	}

	// Vertex buffer Attributes. ---------------------------

	void BuildingShader::setGlVer3dCoordsA(const GLvoid* data) const {
		mw::glVertexAttribPointer(aVerIndex_, 3, GL_FLOAT, GL_FALSE, 0, data);
		mw::glEnableVertexAttribArray(aVerIndex_);
	}

	void BuildingShader::setGlVer3dCoordsA(GLsizei stride, const GLvoid* data) const {
		mw::glVertexAttribPointer(aVerIndex_, 3, GL_FLOAT, GL_FALSE, stride, data);
		mw::glEnableVertexAttribArray(aVerIndex_);
	}

	void BuildingShader::setGlTexCoordsA(const GLvoid* data) const {
		mw::glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, 0, data);
		mw::glEnableVertexAttribArray(aTexIndex_);
	}

	void BuildingShader::setGlTexCoordsA(GLsizei stride, const GLvoid* data) const {
		mw::glVertexAttribPointer(aTexIndex_, 2, GL_FLOAT, GL_FALSE, stride, data);
		mw::glEnableVertexAttribArray(aTexIndex_);
	}	

	// Uniforms. -------------------------------------------

	void BuildingShader::setGlProjectionMatrixU(const mw::Matrix44& matrix) const {
		mw::glUniformMatrix4fv(uProjIndex_, 1, false, matrix.data());
	}

	void BuildingShader::setGlModelMatrixU(const mw::Matrix44& matrix) const {
		mw::glUniformMatrix4fv(uModelIndex_, 1, false, matrix.data());
	}

	void BuildingShader::setGlGlobalCenterPositionU(Position position) const {
		mw::glUniform2f(uGlobalCenterPosIndex_, position.x, position.y);
	}

	void BuildingShader::setGlGlobalHumanPositionU(Position position) const {
		mw::glUniform2f(uGlobalHumanPosIndex_, position.x, position.y);
	}

} // Namespace zombie.
