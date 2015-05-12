#include "glutil.h"

std::ostream& operator << (std::ostream& os, const glm::vec3& v)
{
	os << "( " << v.x << ", " << v.y << ", " << v.z << " )";
	return os;
}

std::ostream& operator << (std::ostream& os, const glm::vec2& v)
{
	os << "( " << v.x << ", " << v.y << " )";
	return os;
}

void reload_shader(Shader *shader, std::string uniforms[], int uniform_sz)
{
	if(uniform_sz == 0)
	{
		shader->id = load_shader_program(shader->vertex_path, shader->fragment_path);
		glUseProgram(shader->id);

		for(std::unordered_map<std::string, GLint>::iterator it = shader->uniforms.begin();
			it != shader->uniforms.end();
			++it)
		{
			GLint pos = glGetUniformLocation(shader->id, (GLchar *)(*it).first.c_str());

			#if DEBUG_BUILD
			std::cout << (*it).first << ": " << pos << std::endl;
			#endif
			shader->uniforms[(*it).first] = pos;
		}
	}
	else
	{
		for(int i = 0; i < uniform_sz; ++i)
		{
			shader->uniforms[uniforms[i].c_str()] = glGetUniformLocation(shader->id, uniforms[i].c_str());
		}
	}
}

void create_shader(Shader *shader, std::string vertex_path, std::string fragment_path, std::string uniforms[], int uniform_sz)
{
	shader->vertex_path = vertex_path;
	shader->fragment_path = fragment_path;
	shader->id = load_shader_program(vertex_path, fragment_path);
	reload_shader(shader, uniforms, uniform_sz);
}

GLuint load_shader_program(std::string vertex_shader_path, std::string fragment_shader_path)
{
	GLuint vertex_shader_id = compile_shader_program(vertex_shader_path, GL_VERTEX_SHADER);
	GLuint fragment_shader_id = compile_shader_program(fragment_shader_path, GL_FRAGMENT_SHADER);

	if(vertex_shader_id == 0)
	{
		std::cerr << "Could not compile vertex shader." << std::endl;
		return 0;
	}

	if(fragment_shader_id == 0)
	{
		std::cerr << "Could not compile fragment shader." << std::endl;
		return 0;
	}

	GLuint program = glCreateProgram();
	glAttachShader(program, vertex_shader_id);
	glAttachShader(program, fragment_shader_id);
	glLinkProgram(program);

	GLint link_status = -1;
	glGetProgramiv(program, GL_LINK_STATUS, &link_status);
	if(link_status == GL_FALSE)
	{
		GLchar message[1024];
		glGetProgramInfoLog(program, sizeof(message), 0, &message[0]);
		std::cout << "[ERRRO " << __LINE__ << " ]: " << vertex_shader_path << "::" << fragment_shader_path << std::endl;
		std::cout << message << std::endl;
	}


	#ifdef DEBUG_BUILD
	std::cout << "Loaded the shader " << vertex_shader_path << "::" << fragment_shader_path << " -> " << program << std::endl;
	#endif

	return program;
}

GLuint compile_shader_program(std::string path, GLenum type)
{
	int file_sz = get_filesize(path);

	char *mem = (char *)malloc(sizeof(char) * file_sz);
	std::ifstream in(path, std::ios::binary);
	in.read(mem, file_sz);
	in.close();

	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, (const GLchar **)&mem, &file_sz);
	glCompileShader(shader);

	GLint compile_status = GL_TRUE;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_status);
	if(compile_status == GL_FALSE)
	{
		GLchar message[1024];
		glGetShaderInfoLog(shader, sizeof(message), 0, &message[0]);
		std::cerr << "[ERROR " << __LINE__ << "]: (" << path << "): " << std::endl << message << std::endl;
		return 0;
	}

	return shader;
}

GLuint load_image_1d(std::string image_path)
{

	SDL_Surface *image = IMG_Load(image_path.c_str());

	GLuint texture_id;
	glGenTextures(1, &texture_id);

	glActiveTexture(GL_TEXTURE0 + 0);
	glBindTexture(GL_TEXTURE_1D, texture_id);

	glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_1D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    std::cout << glm::vec2(image->w, image->h) << std::endl;

    glTexImage1D(GL_TEXTURE_1D,
    			 0,
    			 GL_RGBA,
    			 image->w,
    			 0,
    			 GL_BGRA,
    			 GL_UNSIGNED_BYTE,
    			 image->pixels);

    SDL_FreeSurface(image);

    return texture_id;
}

GLuint load_image_2d(std::string image)
{
	GLuint texture_id;
	glGenTextures(1, &texture_id);
	glBindTexture(GL_TEXTURE_2D, texture_id);

	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_NEAREST);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_NEAREST);

    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,GL_CLAMP_TO_EDGE);
    glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP_TO_EDGE);

    // glTexImage2D(GL_TEXTURE_2D, 0 ,GL_RGBA,image->w,image ->h,0,GL_RGBA,GL_UNSIGNED_BYTE,image->pixels);
    
    return texture_id;
}


std::ifstream::pos_type get_filesize(std::string filename)
{
	std::ifstream in(filename, std::ifstream::ate | std::ifstream::binary);
	if(!in)
	{
		#ifdef DEBUG_BUILD
		std::cout << "[ERROR " << __FILE__ << ":" << __LINE__ << "] Could not read file to get the file size. " << std::endl;
		#endif
		return -2;
	}
	return in.tellg();
}
