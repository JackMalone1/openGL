#pragma once
#include <string>
#include <unordered_map>

struct shaderProgramSource
{
	std::string vertexSource;
	std::string fragmantSource;
};

class Shader {
private:
	std::string m_FilePath;
	unsigned int m_RendererID;
	std::unordered_map<std::string, int> m_UniformLocatioCache;
public:
	Shader(const std::string& filepath);
	~Shader();

	void Bind() const;
	void Unbind() const;

	//set uniforms
	void SetUniform1i(const std::string& name, int value);
	void SetUniform1f(const std::string& name, float value);
	void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3);
private:
	shaderProgramSource ParseShader(const std::string& filepath);
	int GetUniformLocation(const std::string& name);
	unsigned int CompileShader(const std::string& source, unsigned int type);
	unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader);
};
