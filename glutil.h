#ifndef __GLUTIL_H__

#include <iostream>
#include <ostream>
#include <string>
#include <fstream>
#include <SDL2/SDL_opengl.h>
#include <GL/glew.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtx/norm.hpp>
#include <map>
#include <unordered_map>

typedef struct
{
	GLuint id;
	std::string vertex_path;
	std::string fragment_path;
	std::unordered_map<std::string, GLint> uniforms;
} Shader;

typedef struct 
{
	glm::vec3 position;
	glm::vec3 normal;
	glm::vec3 color;
	glm::vec2 uv;
} Vertex3D;

std::ostream& operator << (std::ostream& os, const glm::vec3& v);
std::ostream& operator << (std::ostream& os, const glm::vec2& v);

void reload_shader(Shader *shader, std::string uniforms[], int uniform_sz);
void create_shader(Shader *shader, std::string vertex_path, std::string fragment_path, std::string uniforms[], int uniform_sz);
GLuint load_shader_program(std::string vertex_shader_path, std::string fragment_shader_path);
GLuint compile_shader_program(std::string path, GLenum type);
GLuint load_image_1d(std::string image_path);
GLuint load_image_2d(std::string image);
std::ifstream::pos_type get_filesize(std::string filename);

#endif