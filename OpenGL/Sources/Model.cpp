#include "Model.hpp"

namespace LowRenderer
{

	Model::Model(Resources::Mesh* p_mesh, Resources::Shader* p_shader)
		: mesh(p_mesh),
		shader(p_shader),
		texture(nullptr),
		shaderProgram(shader->GetShaderProgram())
	{
	}

	Model::Model(Resources::Mesh* p_mesh, Resources::Shader* p_shader, Resources::Texture* p_texture)
		: mesh(p_mesh)
		, shader(p_shader)
		, texture(p_texture)
		, shaderProgram(shader->GetShaderProgram())
	{
		isEnable = true;
	}

	Model::Model()
		: mesh(nullptr)
		, shader(nullptr)
		, texture(nullptr)
		, shaderProgram(-1)
	{
		isEnable = false;
	}


	void Model::Draw(const Core::Maths::Mat4& p_transform, const Core::Maths::Mat4& p_mvp) const
	{
		texture->Draw(shaderProgram);
		shader->Draw(p_transform, p_mvp);
		mesh->Draw();
	}
}