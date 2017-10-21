//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/12
//=====================================
#ifndef TESS_POT_H
#define TESS_POT_H

#include <GL/glew.h>
#include <glm/glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;


class VBOTeapotPatch;
class GLSLProgram;

class TessTeapot
{
private:
	GLSLProgram* prog;	//�v���O�����I�u�W�F�N�g
	VBOTeapotPatch *teapot;	//�e�B�[�|�b�g�p�b�`

	//�s��
	mat4 model;
	mat4 view, viewPort;
	mat4 projection;

public:
	TessTeapot(){ ; }
	~TessTeapot();
	void Render();
	void InitShader();
	void Resize(int w, int h);
	void MouseInfo(int button, int state, int x, int y);
	void MotionInfo(int x, int y);

private:
	void SetMatrices();
	void CompileAndLinkShader();
};

#endif