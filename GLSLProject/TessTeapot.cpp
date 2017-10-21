//=====================================
//制作者：佐々木隆貴
//日付：2016/5/12
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
//シェーダー初期化
//=====================================
void TessTeapot::InitShader()
{
	prog = new GLSLProgram();
	CompileAndLinkShader();

	glEnable(GL_DEPTH_TEST);

	mat4 transform = glm::translate(mat4(1.0f), vec3(0.0f, 1.5f, 0.25f));
	teapot = new VBOTeapotPatch();	
	
	//ビュー
	view = glm::lookAt(vec3(2.0f, 4.0f, 2.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

	//ユニフォーム変数に値設定
	prog->setUniform("minTessLevel", 5);	//テッセレーションレベル
	prog->setUniform("maxTessLevel", 15);
	prog->setUniform("maxDepth", 20.0f);	//テッセレーションレベルに影響するモデルビューの距離
	prog->setUniform("minDepth", 2.0f);
	prog->setUniform("line.width", 0.8f);	//メッシュの線の幅の半分
	prog->setUniform("line.color", vec4(0.05f, 0.0f, 0.05f, 1.0f));		//メッシュの線の色
	prog->setUniform("ligh.position", vec4(0.0f, 0.0f, 0.0f, 1.0f));	//ビューに対するライト位置
	prog->setUniform("light.intensity", vec3(1.0f, 1.0f, 1.0f));	//ADS　ライト強度
	prog->setUniform("kd", vec3(0.9f, 0.9f, 1.0f));		//ディフューズ	反射率

	glPatchParameteri(GL_PATCH_VERTICES, 16);
}

//=====================================
//描画
//=====================================
void TessTeapot::Render()
{
	//ウィンドウを塗りつぶす(カラーバッファ、デプスバッファ指定)
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//視点の設定
	camera.Set();

	//ビュー
	view = camera.GetView();

	//モデルの姿勢
	model = mat4(1.0f);
	model *= glm::translate(vec3(0.0, -1.5, 0.0));
	model *= glm::rotate(glm::radians(-90.0f), vec3(1.0f, 0.0f, 0.0f));

	SetMatrices();
	teapot->render();

	glFinish();	//GPU処理が終了するまで関数の実行をブロックする
}

//=====================================
//マテリアル設定
//=====================================
void TessTeapot::SetMatrices()
{
	mat4 mv = view * model;
	prog->setUniform("modelViewMatrix", mv);	//モデルビュー行列
	prog->setUniform("normalMatrix", mat3(vec3(mv[0]), vec3(mv[1]), vec3(mv[2])));	//モデルビュー行列(vvec3との計算のため、3 * 3)
	prog->setUniform("MVP", projection * mv);	//投影  * モデルビュー行列
	prog->setUniform("viewPortMatrix", viewPort);
}

//=====================================
//ウィンドウの拡大縮小に対する処理
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
	//プロジェクション設定
	projection = glm::perspective(glm::radians(60.0f), (float)w / h, 0.3f, 100.0f);
}

//=========================================
//マウスによる入力
//button:押されたボタンの情報
//state:ボタンが押されたか
//x,y:位置
//=========================================-
void TessTeapot::MouseInfo(int button, int state, int x, int y)
{
	camera.MouseInput(button, state, x, y);
}

//=========================================
//マウスドラッグ時
//=========================================
void TessTeapot::MotionInfo(int x, int y)
{
	camera.MouseMotion(x, y);
}

//=====================================
//プログラムオブジェクト作成
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
