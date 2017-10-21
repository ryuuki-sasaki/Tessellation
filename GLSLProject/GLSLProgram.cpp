//=====================================
//制作者：佐々木隆貴
//日付：2016/5/5
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
//シェーダーコンパイル
//fileName:ファイル名
//==============================================================================
void GLSLProgram::compileShader(const char * fileName)
throw(GLSLProgramException) {
	int numExts = sizeof(GLSLShaderInfo::extensions) / sizeof(GLSLShaderInfo::shader_file_extension);	//extensionsの要素数取得

	string ext = getExtension(fileName);	//fileNameの拡張子取得
	GLSLShader::GLSLShaderType type = GLSLShader::VERTEX;	//シェーダータイプ格納
	bool matchFound = false;
	for (int i = 0; i < numExts; i++) {
		//一致する拡張子があれば
		if (ext == GLSLShaderInfo::extensions[i].ext) {
			matchFound = true;
			type = GLSLShaderInfo::extensions[i].type;
			break;
		}
	}

	// 一致する拡張子が無ければ
	if (!matchFound) {
		string msg = "Unrecognized extension: " + ext;
		throw GLSLProgramException(msg);
	}

	// ファイル名とシェーダータイプを渡す
	compileShader(fileName, type);
}

//==============================================================================
//ファイル名の拡張子を返す
//fileName:ファイル名
//==============================================================================
string GLSLProgram::getExtension(const char * name) {
	string nameStr(name);

	size_t loc = nameStr.find_last_of('.');	//name中の.を見つける

	//.が存在すれば
	if (loc != string::npos) {
		return nameStr.substr(loc, string::npos);	//拡張子を返す
	}
	return "";
}

//==============================================================================
//シェーダーコンパイル
//fileName:ファイル名
//type:シェーダータイプ
//==============================================================================
void GLSLProgram::compileShader(const char * fileName,
	GLSLShader::GLSLShaderType type)
	throw(GLSLProgramException)
{
	//パス上にファイルが存在するか
	if (!fileExists(fileName))
	{
		string message = string("Shader: ") + fileName + " not found.";
		throw GLSLProgramException(message);
	}

	//プログラムオブジェクトのハンドルが存在するか
	if (handle <= 0) {
		handle = glCreateProgram();	//プログラムオブジェクトのハンドル作成
		if (handle == 0) {
			throw GLSLProgramException("Unable to create shader program.");
		}
	}

	//ファイル読み込み 
	ifstream inFile(fileName);
	if (!inFile) {
		string message = string("Unable to open: ") + fileName;
		throw GLSLProgramException(message);
	}

	//ファイルの内容を取得します。
	std::stringstream code;
	code << inFile.rdbuf();
	inFile.close();

	//ファイルの内容、シェーダータイプ、ファイル名渡す
	compileShader(code.str(), type, fileName);
}

//==============================================================================
//シェーダーコンパイル
//source:ファイルの内容
//type:シェーダータイプ
//fileName:ファイル名
//==============================================================================
void GLSLProgram::compileShader(const string & source,
	GLSLShader::GLSLShaderType type,
	const char * fileName)
	throw(GLSLProgramException)
{
	//プログラムオブジェクトのハンドルが存在するか
	if (handle <= 0) {
		handle = glCreateProgram();	//プログラムオブジェクトのハンドル作成
		if (handle == 0) {
			throw GLSLProgramException("Unable to create shader program.");
		}
	}

	GLuint shaderHandle = glCreateShader(type);	//シェーダオブジェクトのハンドル作成

	const char * c_code = source.c_str();  //ースコードへのポインタを格納 
	glShaderSource(shaderHandle, 1, &c_code, NULL);	//ソースコードをシェーダオブジェクトにロード

	//シェーダをコンパイル
	glCompileShader(shaderHandle);

	//コンパイルステータスを検証
	int result;
	glGetShaderiv(shaderHandle, GL_COMPILE_STATUS, &result);	//コンパイルステータスを問い合わせる
	if (GL_FALSE == result) {
		int length = 0;
		string logString;
		glGetShaderiv(shaderHandle, GL_INFO_LOG_LENGTH, &length);	//ログの長さを問い合わせる
		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetShaderInfoLog(shaderHandle, length, &written, c_log);	//ログを取り出す
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
		//シェーダーをプログラムオブジェクトにアタッチ
		glAttachShader(handle, shaderHandle);
	}
}

//==========================================================================
//シェーダオブジェクトのリンク
//==========================================================================
void GLSLProgram::link() throw(GLSLProgramException)
{
	//すでにリンクされているかチェック
	if (linked) return;

	//プログラムオブジェクトのハンドルが存在するか
	if (handle <= 0)
		throw GLSLProgramException("Program has not been compiled.");

	//プログラムをリンク
	glLinkProgram(handle);

	//ステータスを検証
	int status = 0;
	glGetProgramiv(handle, GL_LINK_STATUS, &status);	//コンパイルステータスを問い合わせる
	if (GL_FALSE == status) {
		int length = 0;
		string logString;

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);	//ログの長さを問い合わせる

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);	//ログを取り出す
			logString = c_log;
			delete[] c_log;
		}

		throw GLSLProgramException(string("Program link failed:\n") + logString);
	}
	else 
	{
		uniformLocations.clear();	//ユニフォーム変数の場所を格納するmapリストをクリア
		linked = true;	//リンク済み
	}
}

//==========================================================================
//シェーダオブジェクトの使用
//==========================================================================
void GLSLProgram::use() throw(GLSLProgramException)
{
	//リンクが成功していれば
	if (handle <= 0 || (!linked))
		throw GLSLProgramException("Shader has not been linked");
	glUseProgram(handle);	//プログラムをOpenGLパイプラインにインストール
}

//==========================================================================
//プログラムハンドルを返す
//==========================================================================
int GLSLProgram::getHandle()
{
	return handle;
}

//==========================================================================
//リンク済みかどうかを返す
//==========================================================================
bool GLSLProgram::isLinked()
{
	return linked;
}

//==========================================================================
//入力頂点データを頂点シェーダが参照する際に用いる変数の位置を指定
//location:変数の位置
//name:変数名
//memo:
//	プログラムのリンクの前に呼ぶ
//==========================================================================
void GLSLProgram::bindAttribLocation(GLuint location, const char * name)
{
	//locationをシェーダの入力変数nameにバインド
	glBindAttribLocation(handle, location, name);
}

//==========================================================================
//フラグメントシェーダから出力するデータを格納する変数の出力先を指定
//location:カラーバッファ
//name:変数名
//memo:
//	プログラムのリンクの前に呼ぶ
//==========================================================================
void GLSLProgram::bindFragDataLocation(GLuint location, const char * name)
{
	//locationをカラーバッファnameにバインド
	glBindFragDataLocation(handle, location, name);
}

//==========================================================================
//ユニフォーム変数に値を代入
//name:ユニフォーム変数名
//以下の引数:代入値
//==========================================================================
void GLSLProgram::setUniform(const char *name, float x, float y, float z)
{
	GLint loc = getUniformLocation(name);	//nameの場所を求める
	glUniform3f(loc, x, y, z);	//ユニフォーム変数に値を代入
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
///アクティブなユニフォーム変数の表示
//==========================================================================
void GLSLProgram::printActiveUniforms() {
	GLint numUniforms = 0;
	glGetProgramInterfaceiv(handle, GL_UNIFORM, GL_ACTIVE_RESOURCES, &numUniforms);	//アクティブなユニフォーム変数数の問い合わせ

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION, GL_BLOCK_INDEX };

	//個々のアクティブなユニフォーム変数についての情報をプリント
	printf("Active uniforms:\n");
	for (int i = 0; i < numUniforms; ++i) {
		GLint results[4];
		glGetProgramResourceiv(handle, GL_UNIFORM, i, 4, properties, 4, NULL, results);	//i番目のインデックスのユニフォーム変数についての情報取得

		if (results[3] != -1) continue;  // ユニフォームブロックはスキップ
		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(handle, GL_UNIFORM, i, nameBufSize, NULL, name);	//ユニフォーム変数名取得
		printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));	//表示
		delete[] name;
	}
}

//==========================================================================
///アクティブなユニフォームブロックの表示
//==========================================================================
void GLSLProgram::printActiveUniformBlocks() {
	GLint numBlocks = 0;

	glGetProgramInterfaceiv(handle, GL_UNIFORM_BLOCK, GL_ACTIVE_RESOURCES, &numBlocks);	//アクティブなユニフォームブロック数の問い合わせ
	GLenum blockProps[] = { GL_NUM_ACTIVE_VARIABLES, GL_NAME_LENGTH };
	GLenum blockIndex[] = { GL_ACTIVE_VARIABLES };
	GLenum props[] = { GL_NAME_LENGTH, GL_TYPE, GL_BLOCK_INDEX };

	for (int block = 0; block < numBlocks; ++block) {	//ブロック数分ループ
		GLint blockInfo[2];
		glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 2, blockProps, 2, NULL, blockInfo);	//block番目のインデックスのユニフォームブロックについての情報取得
		GLint numUnis = blockInfo[0];	//ブロック内のユニフォーム変数の数代入

		char * blockName = new char[blockInfo[1] + 1];
		glGetProgramResourceName(handle, GL_UNIFORM_BLOCK, block, blockInfo[1] + 1, NULL, blockName);	//ユニフォームブロック名取得
		printf("Uniform block \"%s\":\n", blockName);	//表示
		delete[] blockName;

		GLint * unifIndexes = new GLint[numUnis];
		glGetProgramResourceiv(handle, GL_UNIFORM_BLOCK, block, 1, blockIndex, numUnis, NULL, unifIndexes);	//block番目のインデックスのユニフォームブロック内変数についての情報取得

		for (int unif = 0; unif < numUnis; ++unif) {	//ブロック内のユニフォーム変数分ループ
			GLint uniIndex = unifIndexes[unif];
			GLint results[3];
			glGetProgramResourceiv(handle, GL_UNIFORM, uniIndex, 3, props, 3, NULL, results);	//uniIndexに対応するユニフォーム変数についての情報取得

			GLint nameBufSize = results[0] + 1;
			char * name = new char[nameBufSize];
			glGetProgramResourceName(handle, GL_UNIFORM, uniIndex, nameBufSize, NULL, name);	//ユニフォームブロック内の変数名取得
			printf("    %s (%s)\n", name, getTypeString(results[1]));	//表示
			delete[] name;
		}

		delete[] unifIndexes;
	}
}

//==========================================================================
///アクティブな属性の表示
//==========================================================================
void GLSLProgram::printActiveAttribs() {
	GLint numAttribs;
	glGetProgramInterfaceiv(handle, GL_PROGRAM_INPUT, GL_ACTIVE_RESOURCES, &numAttribs);	//アクティブな属性数の問い合わせ

	GLenum properties[] = { GL_NAME_LENGTH, GL_TYPE, GL_LOCATION };

	//個々のアクティブな属性についての情報をプリント
	printf("Active attributes:\n");
	for (int i = 0; i < numAttribs; ++i) {
		GLint results[3];
		glGetProgramResourceiv(handle, GL_PROGRAM_INPUT, i, 3, properties, 3, NULL, results);	//i番目のインデックスの属性についての情報取得

		GLint nameBufSize = results[0] + 1;
		char * name = new char[nameBufSize];
		glGetProgramResourceName(handle, GL_PROGRAM_INPUT, i, nameBufSize, NULL, name);	//ユニフォーム変数名取得
		printf("%-5d %s (%s)\n", results[2], name, getTypeString(results[1]));	//表示
		delete[] name;
	}
}

//==========================================================================
//引数に対応する文字列を返す
//type:型名
//戻り値：対応する文字列
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
//シェーダープログラムが実行可能か調べる
//memo:
//	描画前に呼ぶ
//==============================================================================
void GLSLProgram::validate() throw(GLSLProgramException)
{
	//リンクされているか
	if (!isLinked())
		throw GLSLProgramException("Program is not linked");

	GLint status;
	glValidateProgram(handle);	//シェーダプログラムが実行可能か調べる
	glGetProgramiv(handle, GL_VALIDATE_STATUS, &status);	//検証結果を格納

	//実行不能であれば
	if (GL_FALSE == status) {
		int length = 0;
		string logString;

		glGetProgramiv(handle, GL_INFO_LOG_LENGTH, &length);		//ログの長さを問い合わせる

		if (length > 0) {
			char * c_log = new char[length];
			int written = 0;
			glGetProgramInfoLog(handle, length, &written, c_log);	//ログを取り出す
			logString = c_log;
			delete[] c_log;
		}

		throw GLSLProgramException(string("Program failed to validate\n") + logString);

	}
}


//==============================================================================
//ユニフォーム変数の場所を求める
//name:ユニフォーム変数名
//戻り値:ユニフォーム変数の場所
//==============================================================================
int GLSLProgram::getUniformLocation(const char * name)
{
	std::map<string, int>::iterator pos;
	pos = uniformLocations.find(name);	//nameの要素を見つけて格納

	//見つからなければ
	if (pos == uniformLocations.end()) {
		uniformLocations[name] = glGetUniformLocation(handle, name);	//ユニフォーム変数の場所を問い合わせして追加
	}

	return uniformLocations[name];
}

//==============================================================================
//ファイルがパス上に存在するか
//fileName:ファイル名
//戻り値:ファイルの有無
//==============================================================================
bool GLSLProgram::fileExists(const string & fileName)
{
	struct stat info;
	int ret = -1;

	ret = stat(fileName.c_str(), &info);	//ファイルが存在するかチェック
	return 0 == ret;
}