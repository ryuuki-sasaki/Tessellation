/*************************************************************************
　　Mouse.cpp
　　
　　Mouse Function Library

　　Version : 1.0
　　Date : Nov. 02, 2007
　　Author : Pocol
*************************************************************************/

//includes
#include <iostream>
#include <cmath>
#include <GL/glut.h>
#include <glm/glm/gtc/matrix_transform.hpp>
#include "Mouse.h"
using namespace std;

//global
char axisLabel[3] = { 'x', 'y', 'z' };
GLfloat axisColor_Red[4] = {1.0, 0.0, 0.0, 1.0};
GLfloat axisColor_Blue[4] = {0.0, 0.0, 1.0, 1.0};
GLfloat axisColor_Green[4] = {0.0, 1.0, 0.0, 1.0};
GLfloat axisColor_Cyan[4] = {0.0, 1.0, 1.0, 1.0};
GLfloat axisColor_Black[4] = {0.0, 0.0, 0.0, 1.0};


///////////////////////////////////////////////////////////////////////////
//MouseButton class
///////////////////////////////////////////////////////////////////////////

//=====================================
//コンストラクタ
//=====================================
MouseButton::MouseButton()
{
	Reset();
}


//=====================================
//デストラクタ
//=====================================
MouseButton::~MouseButton()
{
}

//=====================================
//リセットする
//=====================================
void MouseButton::Reset()
{
	before.x = 0.0;
	before.y = 0.0;
	current.x = 0.0;
	current.y = 0.0;
	after.x = 0.0;
	after.y = 0.0;
	state = None;
}

//=====================================
//コンソール表示
//=====================================
void MouseButton::ConsoleOut()
{
	cout << "before"; 
	before.ConsoleOut();
	cout << "current";
	current.ConsoleOut();
	cout << "after";
	after.ConsoleOut();
}

///////////////////////////////////////////////////////////////////////////
//ViewCamera
///////////////////////////////////////////////////////////////////////////

//=====================================
//コンストラクタ
//dist : 視点の距離
//=====================================
ViewCamera::ViewCamera(double dist)
{
	distance = dist;
	Reset();
}

//=====================================
//~ViewCamera
//Desc : デストラクタ
//=====================================-
ViewCamera::~ViewCamera()
{
}

//=====================================
//Desc：マウスの移動処理
//x, y : マウスの座標
//=====================================
void ViewCamera::MouseMotion( int x, int y ) 
{
	//　左ボタンの処理(カメラの回転)
	if ( left.state == Push )
	{
		//移動量を計算
		left.current.x = (double)x - left.before.x + left.after.x;
		left.current.y = (double)y - left.before.y + left.after.y;

		//回転の範囲を制限
		if ( left.current.y >= 360.0 ) left.current.y -= 360.0;
		else if ( left.current.y < 0.0 ) left.current.y += 360.0;
		
		//現在の角度と移動量から新しい角度を求める
		angle[0] = DegToRad(angle[0] + left.current.x);
		angle[1] = DegToRad(angle[1] + left.current.y);
	}

	//　右ボタンの処理(ズーム)
	if ( right.state == Push )
	{
		//移動量を計算
		right.current.x = (double)x - right.before.x + right.after.x;
		right.current.y  = -(double)y -right.before.y + right.after.y;
	}

	//　中ボタンの処理(カメラの移動)
	if ( middle.state == Push )
	{
		//移動量を計算
		middle.current.x = (double)x -middle.before.x + middle.after.x;
		middle.current.y = (double)y -middle.before.y + middle.after.y;
		//移動量を制限
		translate[0] = middle.current.x * 0.05;
		translate[1] = -middle.current.y * 0.05;
	}
}


//=====================================
//マウスのボタン処理
//state : 操作されたボタン
//button : 操作されたボタン
//x, y : マウスの座標
//=====================================
void ViewCamera::MouseInput( int button, int state, int x, int y ) 
{
	switch( button )
	{
	//　左ボタン
	case GLUT_LEFT_BUTTON :
		if( state == GLUT_DOWN )
		{
			//ボタンを押した座標を格納
			left.before.x = (double)x;
			left.before.y = (double)y;
			//押している状態
			left.state = Push;
		}
		else if( state == GLUT_UP )
		{	
			//ボタンを離した座標を格納
			left.after.x = left.current.x;
			left.after.y = left.current.y;
			//離している状態
			left.state = Release;
		}
		break;

	//　右ボタン
	case GLUT_RIGHT_BUTTON :
		if( state == GLUT_DOWN ) 
		{ 		
			right.before.x = (double)x;
			right.before.y = -(double)y;
			right.state = Push;
		}
		else if( state == GLUT_UP )
		{			
			right.after.x = right.current.x;
			right.after.y = right.current.y;
			right.state = Release;
		}
		break;

	//　中ボタン
	case GLUT_MIDDLE_BUTTON :
		if ( state == GLUT_DOWN )
		{
			middle.before.x = (double)x;
			middle.before.y = (double)y;
			middle.state = Push;
		}
		else if ( state == GLUT_UP )
		{
			middle.after.x = middle.current.x;
			middle.after.y = middle.current.y;
			middle.state = Release;
		}
		break;
	}

}


//=====================================
//パラメータをリセットする
//=====================================
void ViewCamera::Reset()
{
	left.Reset();
	right.Reset();
	middle.Reset();

	angle[0] = DegToRad(45.0);
	angle[1] = DegToRad(45.0);
	angle[2] = 0.0;
	position[0] = -1.0;
	position[1] = 0.25;
	position[2] = 2.0;
	target[0] = 0.0;
	target[1] = 0.0;
	target[2] = 0.0;
	upvector[0] = 0.0;
	upvector[1] = 1.0;
	upvector[2] = 0.0;
	translate[0] = 0.0;
	translate[1] = 0.0;
	translate[2] = 0.0;
}
		

//=====================================
//円盤を描画する
//=====================================
void drawDisk()
{
	GLUquadricObj *qobj;		
	qobj = gluNewQuadric();		//GLUオブジェクトのメモリ確保
	glPushMatrix();				//現在の変換行列を保存
		glRotated( 180, 1.0, 0.0, 0.0 );		//回転
		gluDisk( qobj, 0.0, 0.35, 10, 10 );		//円盤描画
	glPopMatrix();				//保存した変換行列からの復帰
}


//=====================================
//視点の設定
//=====================================
void ViewCamera::Set()
{
	//視点の距離を設定
	double zoom = distance;
	zoom += right.current.y/30.0;

	//　視点位置を決定
	position[0] = sin(angle[0]) * cos(angle[1]) * zoom;
	position[1] = sin(angle[1]) * zoom;
	position[2] = cos(angle[0]) * cos(angle[1]) * zoom;

	//　アップベクトルの設定
	//angleの値が-90～90度の範囲から外れたら上方向ベクトルを-にすることで、オブジェクトの背面も見ることができる
	if( angle[1] > DegToRad(90.0)  &&  angle[1] < DegToRad(270.0) ) upvector[1] = -1.0;
	else upvector[1] = 1.0;

	//　平行移動
	glTranslated( translate[0], translate[1], translate[2] );

	//　視点位置の設定
	view = glm::lookAt(
		vec3(position[0], position[1], position[2]),
		vec3(target[0], target[1], target[2]),
		vec3(upvector[0], upvector[1], upvector[2]));
}

//=====================================
//補助軸の描画
//=====================================
void ViewCamera::RenderSubAxis(int w, int h)
{
	const double zoom = 15.0;	//視点の距離を設定
	double eye[3] = {0.0, 0.0, 0.0};
	
	//　ウィンドウ全体をビューポートにする
	glViewport( w-100, h-100, 100, 100);
	
	//　透視変換行列の設定
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//変換行列に透視変換の行列を乗ずる
	gluPerspective(30.0/*画角*/, 1/*アス比*/, 1/*前方面*/, 100000.0/*後方面*/);

	//　モデルビュー変換の設定
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	//　視点位置を決定
	eye[0] = sin(angle[0])*cos(angle[1])*zoom;
	eye[1] = sin(angle[1])*zoom;
	eye[2] = cos(angle[0])*cos(angle[1])*zoom;

	//　視点位置の設定
	gluLookAt( 
		eye[0], eye[1], eye[2],
		0.0, 0.0, 0.0,
		upvector[0],upvector[1], upvector[2] );

	//　軸の文字
	glPushMatrix();	//現在の変換行列を保存
	glDisable(GL_LIGHTING);	//陰影付けを無効
	glColor4fv(axisColor_Black );	//文字の色
	glRasterPos3d(2.25, 0.0, 0.0);	//文字の位置
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)axisLabel[0]);	//３次元空間にビットマップフォントを一文字配置(10ポイントの比例幅)
	glRasterPos3d(0.0, 2.25, 0.0);		
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)axisLabel[1]);
	glRasterPos3d(0.0, 0.0, 2.25);		
	glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_10, (int)axisLabel[2]);
	glEnable(GL_LIGHTING);	//陰影付けを有効
	glPopMatrix();	//保存した変換行列からの復帰
	

	//　x軸正
	glPushMatrix();	//現在の変換行列を保存
	glColor4fv(axisColor_Red);	//軸の色
	glMaterialfv(GL_FRONT/*正面に材質設定*/, GL_AMBIENT/*環境光に対する反射係数*/, axisColor_Red/*色*/);	//材質の設定
	glMaterialfv(GL_FRONT, GL_DIFFUSE/*拡散反射係数*/, axisColor_Red);
	glMaterialfv(GL_FRONT, GL_SPECULAR/*鏡面反射係数*/, axisColor_Red);
	glTranslated(1.75, 0.0, 0.0);	//平行移動
	glRotated(-90.0, 0.0, 1.0, 0.0);	//回転
	glutSolidCone(0.35, 1.0, 10, 10);	//円錐を描画
	drawDisk();	//円盤を描画
	glPopMatrix();	//保存した変換行列からの復帰

	//　y軸正
	glPushMatrix();	
	glColor4fv(axisColor_Green);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Green);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Green);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Green);
	glTranslated(0.0, 1.75, 0.0);
	glRotated(90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//　z軸正
	glPushMatrix();
	glColor4fv(axisColor_Blue);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Blue);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Blue);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Blue);
	glTranslated(0.0, 0.0, 1.75);
	glRotated(180.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//　キューブ
	glPushMatrix();
	glColor4fv(axisColor_Cyan);
	glMaterialfv(GL_FRONT, GL_AMBIENT, axisColor_Cyan);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, axisColor_Cyan);
	glMaterialfv(GL_FRONT, GL_SPECULAR, axisColor_Cyan);
	glutSolidCube(1.0);	//立方体を描画
	glPopMatrix();

	//　x軸負
	glPushMatrix();
	glTranslated(-1.75, 0.0, 0.0);
	glRotated(90.0, 0.0, 1.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//　y軸負
	glPushMatrix();
	glTranslated(0.0, -1.75, 0.0);
	glRotated(-90.0, 1.0, 0.0, 0.0);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();

	//　z軸負
	glPushMatrix();
	glTranslated(0.0, 0.0, -1.75);
	glutSolidCone(0.35, 1.0, 10, 10);
	drawDisk();
	glPopMatrix();	
}
