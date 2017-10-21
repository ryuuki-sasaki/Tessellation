//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/4/13
//=====================================
//include
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
#include "TessTeapot.h"
using namespace std;

//link
#pragma comment (lib, "glew32.lib")

//global
int windowPositionX = 100;	//�E�B���h�E���J���ʒu
int windowPositionY = 100;
int windowWidth = 512;		//�E�B���h�E��
int windowHeight = 512;		//�E�B���h�E����
char windowTitle[] = "GLSL�v���W�F�N�g";	//�^�C�g��

//prototype
void Initialize();
void Display();
void Idle();
void Reshape(int x, int y);
void Keyboard(unsigned char key, int x, int y);
void Special(int key, int x, int y);
void Mouse(int button, int state, int x, int y);
void Motion(int x, int y);
void PassiveMotion(int x, int y);

TessTeapot diffuse;

//=====================================
//����������
//=====================================
void Initialize()
{
	//GLEW�̏�����
	if (glewInit() != GLEW_OK)
	{
		cout << "Error : GLEW�̏������Ɏ��s���܂���\n";
		exit(0);
	}

	//�E�B���h�E��h��Ԃ��ۂ̐F���w��
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	diffuse.InitShader();
}

//=====================================
//�A�C�h�����O���̏���
//=====================================
void Idle()
{
	glutPostRedisplay();	//glutDisplayFunc()�Ŏw�肳��Ă���`��C�x���g�����s
}

//=========================================
//�E�B���h�E�̊g��k���ɑ΂��鏈��
//w:���݂̃E�B���h�E��
//h:���݂̃E�B���h�E�̍���
//=========================================
void Reshape(int x, int y)
{
	windowWidth = x;
	windowHeight = y;
	//�E�B���h�E�̍ŏ��T�C�Y�𐧌�
	if (windowWidth < 1) windowWidth = 1;
	if (windowHeight < 1) windowHeight = 1;

	//�r���[�|�[�g�̐ݒ�
	glViewport(0, 0, windowWidth, windowHeight);

	diffuse.Resize(x, y);
}

//=========================================
//�E�B���h�E�ւ̕`��
//=========================================
void Display()
{
	//�r���[�|�[�g�̐ݒ�
	glViewport(0, 0, windowWidth, windowHeight);

	diffuse.Render();

	glutSwapBuffers();	//�_�u���o�b�t�@�����O
}

//=========================================
//�}�E�X�ɂ�����
//button:�����ꂽ�{�^���̏��
//state:�{�^���������ꂽ��
//x,y:�ʒu
//=========================================-
void Mouse(int button, int state, int x, int y)
{
	diffuse.MouseInfo(button, state, x, y);
}

//=========================================
//�}�E�X�h���b�O��
//=========================================
void Motion(int x, int y)
{
	diffuse.MotionInfo(x, y);
}

//=========================================
//�}�E�X�ړ���
//=========================================
void PassiveMotion(int x, int y)
{

}

//=========================================
//�L�[�{�[�h������͂��ꂽ�Ƃ�
//key:�^�C�v���ꂽ�L�[�̏��
//x,y:�}�E�X�̈ʒu
//=========================================
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '\033':		//'\033'��ESC��ASCII�R�[�h
		exit(0);
		break;

	default:
		break;
	}
}

//=========================================
//����L�[����
//=========================================
void Special(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_F1:
		break;

	case GLUT_KEY_F2:
		break;

	case GLUT_KEY_F3:
		break;

	case GLUT_KEY_F4:
		break;

	case GLUT_KEY_F5:
		break;

	case GLUT_KEY_F6:
		break;

	case GLUT_KEY_F7:
		break;

	case GLUT_KEY_F8:
		break;

	case GLUT_KEY_F9:
		break;

	case GLUT_KEY_F10:
		break;

	case GLUT_KEY_F11:
		break;

	case GLUT_KEY_F12:
		break;

	case GLUT_KEY_LEFT:
		break;

	case GLUT_KEY_RIGHT:
		break;

	case GLUT_KEY_UP:
		break;

	case GLUT_KEY_DOWN:
		break;

	case GLUT_KEY_PAGE_UP:
		break;

	case GLUT_KEY_PAGE_DOWN:
		break;

	case GLUT_KEY_HOME:
		break;

	case GLUT_KEY_END:
		break;

	case GLUT_KEY_INSERT:
		break;
	}
}

//=====================================
//���C���G���g���|�C���g
//=====================================
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(windowPositionX, windowPositionY);	//�V���ɊJ���E�B���h�E�̈ʒu���w��
	glutInitWindowSize(windowWidth, windowHeight);				//�V���ɊJ���E�B���h�E�̕��ƍ������w��
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	//�f�B�X�v���C�̕\�����[�h��ݒ�(RGBA�w��A�_�u���o�b�t�@�����O)
	glutCreateWindow(windowTitle);		//�E�B���h�E���J��
	glutDisplayFunc(Display);			//�E�B���h�E���ɕ`�悷��֐��ւ̃|�C���^(�E�B���h�E�̍ĕ`��̍ۂɌĂ΂��)
	glutReshapeFunc(Reshape);			//�E�B���h�E�����T�C�Y���ꂽ�Ƃ��Ɏ��s�����֐��ւ̃|�C���^��n��
	glutIdleFunc(Idle);					//�v���O�������ɂȂƂ��Ɏ��s�����֐��ւ̃|�C���^��n��
	glutMouseFunc(Mouse);				//�}�E�X�̃{�^���������ꂽ�Ƃ��Ɏ��s�����֐��ւ̃|�C���^��n��
	glutMotionFunc(Motion);				//�}�E�X�̃{�^���������Ȃ��瓮�������Ƃ��Ɏ��s�����֐��ւ̃|�C���^��n��
	//glutPassiveMotionFunc(PassiveMotion);	//�}�E�X�̃{�^�����������ɓ��������Ƃ��Ɏ��s�����֐��ւ̃|�C���^��n��
	//glutKeyboardFunc(Keyboard);			//�L�[���^�C�v���ꂽ�Ƃ��Ɏ��s�����֐��ւ̃|�C���^��n��
	//glutSpecialFunc(Special);			//����L�[���^�C�v���ꂽ�Ƃ��Ɏ��s�����֐��ւ̃|�C���^��n��

	Initialize(); 

	glutMainLoop();

	return 0;
}
