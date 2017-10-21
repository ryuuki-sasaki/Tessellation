//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/5
//=====================================
#include "glslprogram.h"
#include <fstream>
#include <sstream>
#include <sys/stat.h>

using std::ifstream;

namespace GLSLShaderInfo {
	struct shader_file_extension {
		const char *ext;
		GLSLShader::GLSLShaderType type;
	};

	struct shader_file_extension extensions[] =
	{
		{ ".vs", GLSLShader::VERTEX },
		{ ".vert", GLSLShader::VERTEX },
		{ ".gs", GLSLShader::GEOMETRY },
		{ ".geom", GLSLShader::GEOMETRY },
		{ ".tcs", GLSLShader::TESS_CONTROL },
		{ ".tes", GLSLShader::TESS_EVALUATION },
		{ ".fs", GLSLShader::FRAGMENT },
		{ ".frag", GLSLShader::FRAGMENT },
		{ ".cs", GLSLShader::COMPUTE }
	};
}

GLSLProgram::GLSLProgram() : handle(0), linked(false) { }

GLSLProgram::~GLSLProgram() {
	if (handle == 0) return;

	GLint numShaders = 0;
	glGetProgramiv(handle, GL_ATTACHED_SHADERS, &numShaders);

	GLuint * shaderNames = new GLuint[numShaders];
	glGetAttachedShaders(handle, numShaders, NULL, shaderNames);

	for (int i = 0; i < numShaders; i++)
		glDeleteShader(shaderNames[i]);

	glDeleteProgram(handle);

	delete[] shaderNames;
}

//==============================================================================
//�V�F�[�_�[�R���p�C��
//fileName:�t�@�C����
//==============================================================================
void GLSLProgram::compileShader(const char * fileName)
throw(GLSLProgramException) {
	int numExts = sizeof(GLSLShaderInfo::extensions) / sizeof(GLSLShaderInfo::shader_file_extension);	//extensions�̗v�f���擾

	string ext = getExtension(fileName);	//fileName�̊g���q�擾
	GLSLShader::GLSLShaderType type = GLSLShader::VERTEX;	//�V�F�[�_�[�^�C�v�i�[
	bool matchFound = false;
	for (int i = 0; i < numExts; i++) {
		//��v����g���q�������
		if (ext == GLSLShaderInfo::extensions[i].ext) {
			matchFound = true;
			type = GLSLShaderInfo::extensions[i].type;
			break;
		}
	}

	// ��v����g���q���������
	if (!matchFound) {
		string msg = "Unrecognized extension: " + ext;
		throw GLSLProgramException(msg);
	}

	// �t�@�C�����ƃV�F�[�_�[�^�C�v��n��
	compileShader(fileName, type);
}

//==============================================================================
//�t�@�C�����̊g���q��Ԃ�
//fileName:�t�@�C����
//==============================================================================
string GLSLProgram::getExtension(const char * name) {
	string nameStr(name);

	size_t loc = nameStr.find_last_of('.');	//name����.��������

	//.�����݂����
	if (loc != string::npos) {
		return nameStr.substr(loc, string::npos);	//�g���q��Ԃ�
	}
	return "";
}

//==============================================================================
//�V�F�[�_�[�R���p�C��
//fileName:�t�@�C����
//type:�V�F�[�_�[�^�C�v
//==============================================================================
void GLSLProgram::compileShader(const char * fileName,
	GLSLShader::GLSLShaderType type)
	throw(GLSLProgramException)
{
	//�p�X��Ƀt�@�C�������݂��邩
	if (!fileExists(fileName))
	{
		string message = string("Shader: ") + fileName + " not found.";
		throw GLSLProgramException(message);
	}

	//�v���O�����I�u�W�F�N�g�̃n���h�������݂��邩
	if (handle <= 0) {
		handle = glCreateProgram();	//�v���O�����I�u�W�F�N�g�̃n���h���쐬
		if (handle == 0) {
			throw GLSLProgramException("Unable to create shader program.");
		}
	}

	//�t�@�C���ǂݍ��� 
	ifstream inFile(fileName);
	if (!inFile) {
		string message = string("Unable to open: ") + fileName;
		throw GLSLProgramException(message);
	}

	//�t�@�C���̓��e���擾���܂��B
	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();

	//�t�@�C���̓��e�A�V�F�[�_�[�^�C�v�A�t�@�C�����n��
	compileShader(code.str(), type, fileName);
}

//==============================================================================
//�V�F�[�_�[�R���p�C��
//source:�t�@�C���̓��e
//type:�V�F�[�_�[�^�C�v
//fileName:�t�@�C����
//==============================================================================
void GLSLProgram::compileShader(const string & source,
	GLSLShader::GLSLShaderType type,
	const char * fileName)
	throw(GLSLProgramException)
{
	//�v���O�����I�u�W�F�N�g�̃n���h�������݂��邩
	if (handle <= 0) {
		handle = glCreateProgram();	//�v���O�����I�u�W�F�N�g�̃n���h���쐬
		if (handle == 0) {
			throw GLSLProgramException("Unable to create shader program.");
		}
	}

	GLuint shaderHandle = glCreateShader(type);	//�V�F�[�_�I�u�W�F�N�g�̃n���h���쐬

	const char * c_code = source.c_str();  //�[�X�R�[�h�ւ̃|�C���^���i�[ 
	glShaderSource(shaderHandle, 1, &c_code, NULL);	//�\�[�X�R�[�h���V�F�[�_�I�u�W�F�N�g�Ƀ��[�h

	//�V�F�[�_���R���p�C��
	glCompileShader(shaderHandle);

	//�R���p�C���X�e�[�^�X������
	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);	//�R���p�C���X�e�[�^�X��₢���킹��
	if (GL_FALSE == result) {
		int length = 0;
		string logString;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);	//���O�̒�����₢���킹��
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);	//���O�����o��
			logString = c_log;
			delete[] c_log;
		}
		string msg;
		if (fileName) {
			msg = string(fileName) + ": shader compliation failed\n";
		}
		else {
			msg = "Shader compilation failed.\n";
		}
		msg += logString;

		throw GLSLProgramException(msg);

	}
	else 
	{
		//�V�F�[�_�[���v���O�����I�u�W�F�N�g�ɃA�^�b�`
		glAttachShader(handle, shaderHandle);
	}
}

//==========================================================================
//�V�F�[�_�I�u�W�F�N�g�̃����N
//==========================================================================
void GLSLProgram::link() throw(GLSLProgramException)
{
	//���łɃ����N����Ă��邩�`�F�b�N
	if (linked) return;

	//�v���O�����I�u�W�F�N�g�̃n���h�������݂��邩
	if (handle <= 0)
		throw GLSLProgramException("Program has not been compiled.");

	//�v���O�����������N
	glLinkProgram(handle);

	//�X�e�[�^�X������
	int status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);	//�R���p�C���X�e�[�^�X��₢���킹��
	if (GL_FALSE == status) {
		int length = 0;
		string logString;

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);	//���O�̒�����₢���킹��

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);	//���O�����o��
			logString = c_log;
			delete[] c_log;
		}

		throw GLSLProgramException(string("Program link failed:\n") + logString);
	}
	else 
	{
		uniformLocations.clear();	//���j�t�H�[���ϐ��̏ꏊ���i�[����map���X�g���N���A
		linked = true;	//�����N�ς�
	}
}

//==========================================================================
//�V�F�[�_�I�u�W�F�N�g�̎g�p
//==========================================================================
void GLSLProgram::use() throw(GLSLProgramException)
{
	//�����N���������Ă����
	if (handle <= 0 || (!linked))
		throw GLSLProgramException("Shader has not been linked");
	glUseProgram(handle);	//�v���O������OpenGL�p�C�v���C���ɃC���X�g�[��
}

//==========================================================================
//�v���O�����n���h����Ԃ�
//==========================================================================
int GLSLProgram::getHandle()
{
	return handle;
}

//==========================================================================
//�����N�ς݂��ǂ�����Ԃ�
//==========================================================================
bool GLSLProgram::isLinked()
{
	return linked;
}

//==========================================================================
//���͒��_�f�[�^�𒸓_�V�F�[�_���Q�Ƃ���ۂɗp����ϐ��̈ʒu���w��
//location:�ϐ��̈ʒu
//name:�ϐ���
//memo:
//	�v���O�����̃����N�̑O�ɌĂ�
//==========================================================================
void GLSLProgram::bindAttribLocation(GLuint location, const char * name)
{
	//location���V�F�[�_�̓��͕ϐ�name�Ƀo�C���h
	glBindAttribLocation(handle, location, name);
}

//==========================================================================
//�t���O�����g�V�F�[�_����o�͂���f�[�^���i�[����ϐ��̏o�͐���w��
//location:�J���[�o�b�t�@
//name:�ϐ���
//memo:
//	�v���O�����̃����N�̑O�ɌĂ�
//==========================================================================
void GLSLProgram::bindFragDataLocation(GLuint location, const char * name)
{
	//location���J���[�o�b�t�@name�Ƀo�C���h
	glBindFragDataLocation(handle, location, name);
}

//==========================================================================
//���j�t�H�[���ϐ��ɒl����
//name:���j�t�H�[���ϐ���
//�ȉ��̈���:����l
//==========================================================================
void GLSLProgram::setUniform(const char *name, float x, float y, float z)
{
	GLint loc = getUniformLocation(name);	//name�̏ꏊ�����߂�
	glUniform3f(loc, x, y, z);	//���j�t�H�[���ϐ��ɒl����
}

void GLSLProgram::setUniform(const char *name, const vec3 & v)
{
	this->setUniform(name, v.x, v.y, v.z);
}

void GLSLProgram::setUniform(const char *name, const vec4 & v)
{
	GLint loc = getUniformLocation(name);
	glUniform4f(loc, v.x, v.y, v.z, v.w);
}

void GLSLProgram::setUniform(const char *name, const vec2 & v)
{
	GLint loc = getUniformLocation(name);
	glUniform2f(loc, v.x, v.y);
}

void GLSLProgram::setUniform(const char *name, const mat4 & m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix4fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::setUniform(const char *name, const mat3 & m)
{
	GLint loc = getUniformLocation(name);
	glUniformMatrix3fv(loc, 1, GL_FALSE, &m[0][0]);
}

void GLSLProgram::setUniform(const char *name, float val)
{
	GLint loc = getUniformLocation(name);
	glUniform1f(loc, val);
}

void GLSLProgram::setUniform(const char *name, int val)
{
	GLint loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

void GLSLProgram::setUniform(const char *name, GLuint val)
{
	GLint loc = getUniformLocation(name);
	glUniform1ui(loc, val);
}

void GLSLProgram::setUniform(const char *name, bool val)
{
	int loc = getUniformLocation(name);
	glUniform1i(loc, val);
}

//==========================================================================
///�A�N�e�B�u�ȃ��j�t�H�[���ϐ��̕\��
//==========================================================================
void GLSLProgram::printActiveUniforms() {
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);	//�A�N�e�B�u�ȃ��j�t�H�[���ϐ����̖₢���킹

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	//�X�̃A�N�e�B�u�ȃ��j�t�H�[���ϐ��ɂ��Ă̏����v�����g
	printf("Active uniforms:\n");
	for (int i = 0; i < numUniforms; ++i) {
		GLint results[4];
		glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL, results);	//i�Ԗڂ̃C���f�b�N�X�̃��j�t�H�[���ϐ��ɂ��Ă̏��擾

		if (results[3] != -1) continue;  // ���j�t�H�[���u���b�N�̓X�L�b�v
		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL, name);	//���j�t�H�[���ϐ����擾
		printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));	//�\��
		delete[] name;
	}
}

//==========================================================================
///�A�N�e�B�u�ȃ��j�t�H�[���u���b�N�̕\��
//==========================================================================
void GLSLProgram::printActiveUniformBlocks() {
	GLint numBlocks = 0;

	glGetProgramInterfaceiv(handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);	//�A�N�e�B�u�ȃ��j�t�H�[���u���b�N���̖₢���킹
	GLenum blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
	GLenum blockIndex[] = { GL_ACTIVE_VARIABLES };
	GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

	for (int block = 0; block < numBlocks; ++block) {	//�u���b�N�������[�v
		GLint blockInfo[2];
		glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);	//block�Ԗڂ̃C���f�b�N�X�̃��j�t�H�[���u���b�N�ɂ��Ă̏��擾
		GLint numUnis = blockInfo[0];	//�u���b�N���̃��j�t�H�[���ϐ��̐����

		char * blockName = new char[blockInfo[1] + 1];
		glGetProgramResourceName(handle, GL_UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);	//���j�t�H�[���u���b�N���擾
		printf("Uniform block \"%s\":\n", blockName);	//�\��
		delete[] blockName;

		GLint * unifIndexes = new GLint[numUnis];
		glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);	//block�Ԗڂ̃C���f�b�N�X�̃��j�t�H�[���u���b�N���ϐ��ɂ��Ă̏��擾

		for (int unif = 0; unif < numUnis; ++unif) {	//�u���b�N���̃��j�t�H�[���ϐ������[�v
			GLint uniIndex = unifIndexes[unif];
			GLint results[3];
			glGetProgramResourceiv(handle, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results);	//uniIndex�ɑΉ����郆�j�t�H�[���ϐ��ɂ��Ă̏��擾

			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			glGetProgramResourceName(handle, GL_UNIFORM, uniIndex, nameBufSize, NULL, name);	//���j�t�H�[���u���b�N���̕ϐ����擾
			printf("    %s (%s)\n", name, getTypeString(results[1]));	//�\��
			delete[] name;
		}

		delete[] unifIndexes;
	}
}

//==========================================================================
///�A�N�e�B�u�ȑ����̕\��
//==========================================================================
void GLSLProgram::printActiveAttribs() {
	GLint numAttribs;
	glGetProgramInterfaceiv(handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);	//�A�N�e�B�u�ȑ������̖₢���킹

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	//�X�̃A�N�e�B�u�ȑ����ɂ��Ă̏����v�����g
	printf("Active attributes:\n");
	for (int i = 0; i < numAttribs; ++i) {
		GLint results[3];
		glGetProgramResourceiv(handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);	//i�Ԗڂ̃C���f�b�N�X�̑����ɂ��Ă̏��擾

		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);	//���j�t�H�[���ϐ����擾
		printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));	//�\��
		delete[] name;
	}
}

//==========================================================================
//�����ɑΉ����镶�����Ԃ�
//type:�^��
//�߂�l�F�Ή����镶����
//==========================================================================
const char * GLSLProgram::getTypeString(GLenum type) {

	switch (type) {
	case GL_FLOAT:
		return "float";
	case GL_FLOAT_VEC2:
		return "vec2";
	case GL_FLOAT_VEC3:
		return "vec3";
	case GL_FLOAT_VEC4:
		return "vec4";
	case GL_DOUBLE:
		return "double";
	case GL_INT:
		return "int";
	case GL_UNSIGNED_INT:
		return "unsigned int";
	case GL_BOOL:
		return "bool";
	case GL_FLOAT_MAT2:
		return "mat2";
	case GL_FLOAT_MAT3:
		return "mat3";
	case GL_FLOAT_MAT4:
		return "mat4";
	default:
		return "?";
	}
}

//==============================================================================
//�V�F�[�_�[�v���O���������s�\�����ׂ�
//memo:
//	�`��O�ɌĂ�
//==============================================================================
void GLSLProgram::validate() throw(GLSLProgramException)
{
	//�����N����Ă��邩
	if (!isLinked())
		throw GLSLProgramException("Program is not linked");

	GLint status;
	glValidateProgram(handle);	//�V�F�[�_�v���O���������s�\�����ׂ�
	glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);	//���،��ʂ��i�[

	//���s�s�\�ł����
	if (GL_FALSE == status) {
		int length = 0;
		string logString;

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);		//���O�̒�����₢���킹��

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);	//���O�����o��
			logString = c_log;
			delete[] c_log;
		}

		throw GLSLProgramException(string("Program failed to validate\n") + logString);

	}
}


//==============================================================================
//���j�t�H�[���ϐ��̏ꏊ�����߂�
//name:���j�t�H�[���ϐ���
//�߂�l:���j�t�H�[���ϐ��̏ꏊ
//==============================================================================
int GLSLProgram::getUniformLocation(const char * name)
{
	std::map<string, int>::iterator pos;
	pos = uniformLocations.find(name);	//name�̗v�f�������Ċi�[

	//������Ȃ����
	if (pos == uniformLocations.end()) {
		uniformLocations[name] = glGetUniformLocation(handle, name);	//���j�t�H�[���ϐ��̏ꏊ��₢���킹���Ēǉ�
	}

	return uniformLocations[name];
}

//==============================================================================
//�t�@�C�����p�X��ɑ��݂��邩
//fileName:�t�@�C����
//�߂�l:�t�@�C���̗L��
//==============================================================================
bool GLSLProgram::fileExists(const string & fileName)
{
	struct stat info;
	int ret = -1;

	ret = stat(fileName.c_str(), &info);	//�t�@�C�������݂��邩�`�F�b�N
	return 0 == ret;
}