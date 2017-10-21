/***************************************************************************
		Mouse.h

		Mouse Function Library

		Version : 1.0
		Date : Nov. 02, 2007
		Author : Pocol
****************************************************************************/

#ifndef _MOUSE_H_INCLUDED_
#define _MOUSE_H_INCLUDED_

//
//�@include
//
#include <iostream>
#include <glm/glm/glm.hpp>
using glm::vec2;
using glm::vec3;
using glm::vec4;
using glm::mat4;
using glm::mat3;
using namespace std;


//
//�@global
//
const double PI = 3.14159265358979323846264338327;
template<class T> static inline T RadToDeg(T rad) { return ( (rad)*(180.0/PI) ); }
template<class T> static inline T DegToRad(T deg) { return ( (deg)*(PI/180.0) ); }
typedef enum MState	{ Push/*�����ꂽ*/, Release/*�����ꂽ*/, None/*���I��*/ };

//////////////////////////////////////////////////////////////////////////
//�@�@MPosition class
//////////////////////////////////////////////////////////////////////////
class MPosition
{
public:
	double x;
	double y;

	MPosition(double nx=0.0, double ny=0.0) { x=nx; y=ny; }
	~MPosition() {}
	void Reset() { x = 0.0, y=0.0; }	//���W�l��������
	void ConsoleOut() { cout << "(x, y) = (" << x << ", " << y << ")\n"; }	//�}�E�X�̍��W�l���o��
};


//////////////////////////////////////////////////////////////////////////
//�@�@MouseButton class
//////////////////////////////////////////////////////////////////////////
class MouseButton
{
public:
	MPosition before;	//�}�E�X���������Ƃ��̍��W
	MPosition current;	//�}�E�X���ړ����̍��W
	MPosition after;	//�}�E�X�𗣂����Ƃ��̍��W
	MState state;		//�{�^���̏��

	MouseButton();
	~MouseButton();
	void Reset();		//�����o�̒l��������
	void ConsoleOut();	//MPosition�^�����o�̒l���o��
};

//////////////////////////////////////////////////////////////////////////
//�@�@ViewCamara class
//////////////////////////////////////////////////////////////////////////
class ViewCamera
{
public:
	MouseButton right;	//�}�E�X�̉E�{�^���̏��
	MouseButton left;	//�}�E�X�̍��{�^���̏��
	MouseButton middle;	//�}�E�X�̒����{�^���̏��

	mat4 view;	//�r���[���W

	double distance;		//���_�̋���
	double angle[3];		//���_�̊p�x
	double position[3];		//���_�̈ʒu
	double target[3];		//�ڕW�̈ʒu
	double upvector[3];		//�E�B���h�E�ɕ\�������I�u�W�F�N�g�̏�����x�N�g��
	double translate[3];	//�ϊ��s��悶�镽�s�ړ��̒l

	ViewCamera(double dist=5.0);
	~ViewCamera();
	void Reset();			//�����o�̒l��������
	void MouseInput(int button, int state, int x, int y);	//�}�E�X�̃{�^������
	void MouseMotion(int x, int y);							//�}�E�X�̈ړ�����
	void Set();												//���_�̐ݒ�
	void RenderSubAxis(int w, int h);						//�⏕���̕`��
	mat4 GetView(){ return view; }
};



#endif		//�@_MOUSE_WIN_H_INCLUDED_
