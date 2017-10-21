//=====================================
//制作者：佐々木隆貴
//日付：2016/4/13
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
int windowPositionX = 100;	//ウィンドウを開く位置
int windowPositionY = 100;
int windowWidth = 512;		//ウィンドウ幅
int windowHeight = 512;		//ウィンドウ高さ
char windowTitle[] = "GLSLプロジェクト";	//タイトル

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
//初期化処理
//=====================================
void Initialize()
{
	//GLEWの初期化
	if (glewInit() != GLEW_OK)
	{
		cout << "Error : GLEWの初期化に失敗しました\n";
		exit(0);
	}

	//ウィンドウを塗りつぶす際の色を指定
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);

	diffuse.InitShader();
}

//=====================================
//アイドリング時の処理
//=====================================
void Idle()
{
	glutPostRedisplay();	//glutDisplayFunc()で指定されている描画イベントを実行
}

//=========================================
//ウィンドウの拡大縮小に対する処理
//w:現在のウィンドウ幅
//h:現在のウィンドウの高さ
//=========================================
void Reshape(int x, int y)
{
	windowWidth = x;
	windowHeight = y;
	//ウィンドウの最小サイズを制限
	if (windowWidth < 1) windowWidth = 1;
	if (windowHeight < 1) windowHeight = 1;

	//ビューポートの設定
	glViewport(0, 0, windowWidth, windowHeight);

	diffuse.Resize(x, y);
}

//=========================================
//ウィンドウへの描画
//=========================================
void Display()
{
	//ビューポートの設定
	glViewport(0, 0, windowWidth, windowHeight);

	diffuse.Render();

	glutSwapBuffers();	//ダブルバッファリング
}

//=========================================
//マウスによる入力
//button:押されたボタンの情報
//state:ボタンが押されたか
//x,y:位置
//=========================================-
void Mouse(int button, int state, int x, int y)
{
	diffuse.MouseInfo(button, state, x, y);
}

//=========================================
//マウスドラッグ時
//=========================================
void Motion(int x, int y)
{
	diffuse.MotionInfo(x, y);
}

//=========================================
//マウス移動時
//=========================================
void PassiveMotion(int x, int y)
{

}

//=========================================
//キーボードから入力されたとき
//key:タイプされたキーの情報
//x,y:マウスの位置
//=========================================
void Keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '\033':		//'\033'はESCのASCIIコード
		exit(0);
		break;

	default:
		break;
	}
}

//=========================================
//特殊キー処理
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
//メインエントリポイント
//=====================================
int main(int argc, char *argv[])
{
	glutInit(&argc, argv);
	glutInitWindowPosition(windowPositionX, windowPositionY);	//新たに開くウィンドウの位置を指定
	glutInitWindowSize(windowWidth, windowHeight);				//新たに開くウィンドウの幅と高さを指定
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);	//ディスプレイの表示モードを設定(RGBA指定、ダブルバッファリング)
	glutCreateWindow(windowTitle);		//ウィンドウを開く
	glutDisplayFunc(Display);			//ウィンドウ内に描画する関数へのポインタ(ウィンドウの再描画の際に呼ばれる)
	glutReshapeFunc(Reshape);			//ウィンドウがリサイズされたときに実行される関数へのポインタを渡す
	glutIdleFunc(Idle);					//プログラムが暇なときに実行される関数へのポインタを渡す
	glutMouseFunc(Mouse);				//マウスのボタンが押されたときに実行される関数へのポインタを渡す
	glutMotionFunc(Motion);				//マウスのボタンを押しながら動かしたときに実行される関数へのポインタを渡す
	//glutPassiveMotionFunc(PassiveMotion);	//マウスのボタンを押さずに動かしたときに実行される関数へのポインタを渡す
	//glutKeyboardFunc(Keyboard);			//キーがタイプされたときに実行される関数へのポインタを渡す
	//glutSpecialFunc(Special);			//特殊キーがタイプされたときに実行される関数へのポインタを渡す

	Initialize(); 

	glutMainLoop();

	return 0;
}
