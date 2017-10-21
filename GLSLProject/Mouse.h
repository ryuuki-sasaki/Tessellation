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
//　include
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
//　global
//
const double PI = 3.14159265358979323846264338327;
template<class T> static inline T RadToDeg(T rad) { return ( (rad)*(180.0/PI) ); }
template<class T> static inline T DegToRad(T deg) { return ( (deg)*(PI/180.0) ); }
typedef enum MState	{ Push/*押された*/, Release/*離された*/, None/*未選択*/ };

//////////////////////////////////////////////////////////////////////////
//　　MPosition class
//////////////////////////////////////////////////////////////////////////
class MPosition
{
public:
	double x;
	double y;

	MPosition(double nx=0.0, double ny=0.0) { x=nx; y=ny; }
	~MPosition() {}
	void Reset() { x = 0.0, y=0.0; }	//座標値を初期化
	void ConsoleOut() { cout << "(x, y) = (" << x << ", " << y << ")\n"; }	//マウスの座標値を出力
};


//////////////////////////////////////////////////////////////////////////
//　　MouseButton class
//////////////////////////////////////////////////////////////////////////
class MouseButton
{
public:
	MPosition before;	//マウスを押したときの座標
	MPosition current;	//マウスを移動中の座標
	MPosition after;	//マウスを離したときの座標
	MState state;		//ボタンの状態

	MouseButton();
	~MouseButton();
	void Reset();		//メンバの値を初期化
	void ConsoleOut();	//MPosition型メンバの値を出力
};

//////////////////////////////////////////////////////////////////////////
//　　ViewCamara class
//////////////////////////////////////////////////////////////////////////
class ViewCamera
{
public:
	MouseButton right;	//マウスの右ボタンの情報
	MouseButton left;	//マウスの左ボタンの情報
	MouseButton middle;	//マウスの中央ボタンの情報

	mat4 view;	//ビュー座標

	double distance;		//視点の距離
	double angle[3];		//視点の角度
	double position[3];		//視点の位置
	double target[3];		//目標の位置
	double upvector[3];		//ウィンドウに表示されるオブジェクトの上方向ベクトル
	double translate[3];	//変換行列乗じる平行移動の値

	ViewCamera(double dist=5.0);
	~ViewCamera();
	void Reset();			//メンバの値を初期化
	void MouseInput(int button, int state, int x, int y);	//マウスのボタン処理
	void MouseMotion(int x, int y);							//マウスの移動処理
	void Set();												//視点の設定
	void RenderSubAxis(int w, int h);						//補助軸の描画
	mat4 GetView(){ return view; }
};



#endif		//　_MOUSE_WIN_H_INCLUDED_
