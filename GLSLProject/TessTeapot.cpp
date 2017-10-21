//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/12
//=====================================
#include "TessTeapot.h"
#include "VBOTeapotPatch.h"
#include "glslprogram.h"
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include <glm/glm/gtc/matrix_transform.hpp>
#include <glm/glm/gtx/transform.hpp>
#include "Mouse.h"
#include <iostream>
using std::cerr;
using std::endl;

//link
#pragma comment (lib, "glew32.lib")

ViewCamera camera(5.0);

TessTeapot::~TessTeapot()
{
	delete teapot;
	delete prog;
	teapot = 0;
	prog = 0;
}

//=====================================
//�V�F�[�_�[������
//=====================================
void TessTeapot::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	mat4 transform = glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f));
	teapot = new VBOTeapotPatch();	
	
	//�r���[
	view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//���j�t�H�[���ϐ��ɒl�ݒ�
	prog->setUniform("minTessLevel", 5);	//�e�b�Z���[�V�������x��
	prog->setUniform("maxTessLevel", 15);
	prog->setUniform("maxDepth", 20.0f);	//�e�b�Z���[�V�������x���ɉe�����郂�f���r���[�̋���
	prog->setUniform("minDepth", 2.0f);
	prog->setUniform("line.width", 0.8f);	//���b�V���̐��̕��̔���
	prog->setUniform("line.color", vec4(0.05f, 0.0f, 0.05f, 1.0f));		//���b�V���̐��̐F
	prog->setUniform("ligh.position", vec4(0.0f, 0.0f, 0.0f, 1.0f));	//�r���[�ɑ΂��郉�C�g�ʒu
	prog->setUniform("light.intensity", vec3(1.0f, 1.0f, 1.0f));	//ADS�@���C�g���x
	prog->setUniform("kd", vec3(0.9f, 0.9f, 1.0f));		//�f�B�t���[�Y	���˗�

	glPatchParameteri(GL_PATCH_VERTICES, 16);
}

//=====================================
//�`��
//=====================================
void TessTeapot::Render()
{
	//�E�B���h�E��h��Ԃ�(�J���[�o�b�t�@�A�f�v�X�o�b�t�@�w��)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//���_�̐ݒ�
	camera.Set();

	//�r���[
	view = camera.GetView();

	//���f���̎p��
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0, -1.5, 0.0));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	SetMatrices();
	teapot->render();

	glFinish();	//GPU�������I������܂Ŋ֐��̎��s���u���b�N����
}

//=====================================
//�}�e���A���ݒ�
//=====================================
void TessTeapot::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//���f���r���[�s��
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//���f���r���[�s��(vvec3�Ƃ̌v�Z�̂��߁A3 * 3)
	prog->setUniform("MVP", projection * mv);	//���e  * ���f���r���[�s��
	prog->setUniform("viewPortMatrix", viewPort);
}

//=====================================
//�E�B���h�E�̊g��k���ɑ΂��鏈��
//=====================================
void TessTeapot::Resize(int w, int h)
{
	glViewport(0, 0, w, h);
	float w2 = w / 2.0f;
	float h2 = h / 2.0f;
	viewPort = mat4(vec4(w2, 0.0f, 0.0f, 0.0f),
		vec4(0.0f, h2, 0.0f, 0.0f),
		vec4(0.0f, 0.0f, 1.0f, 0.0f),
		vec4(w2 + 0, h2 + 0, 0.0f, 1.0f));
	//�v���W�F�N�V�����ݒ�
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

//=========================================
//�}�E�X�ɂ�����
//button:�����ꂽ�{�^���̏��
//state:�{�^���������ꂽ��
//x,y:�ʒu
//=========================================-
void TessTeapot::MouseInfo(int button, int state, int x, int y)
{
	camera.MouseInput(button, state, x, y);
}

//=========================================
//�}�E�X�h���b�O��
//=========================================
void TessTeapot::MotionInfo(int x, int y)
{
	camera.MouseMotion(x, y);
}

//=====================================
//�v���O�����I�u�W�F�N�g�쐬
//=====================================
void TessTeapot::CompileAndLinkShader()
{
	try {
		prog->compileShader("shader/tessteapot.vert", GLSLShader::VERTEX);
		prog->compileShader("shader/tessteapot.frag", GLSLShader::FRAGMENT);
		prog->compileShader("shader/wireframe.gs", GLSLShader::GEOMETRY);
		prog->compileShader("shader/tessteapot.tes", GLSLShader::TESS_EVALUATION);
		prog->compileShader("shader/tessteapot.tcs", GLSLShader::TESS_CONTROL);
		prog->link();
		prog->use();
		prog->printActiveUniforms();
		prog->printActiveUniformBlocks();
		prog->printActiveAttribs();
	}
	catch (GLSLProgramException &e) {
		cerr << e.what() << endl;
		exit(EXIT_FAILURE);
	}
}
