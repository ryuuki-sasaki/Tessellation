//=====================================
//制作者：佐々木隆貴
//日付：2016/3/30
//=====================================
#ifndef _DDS_LOADER_H_
#define _DDS_LOADER_H_

//include
#include <iostream>
#include <GL/glew.h>
#include <GL/glut.h>
using namespace std;

//link
#pragma comment(lib, "glew32.lib")

//define
#define FOURCC_DXT1 '1TXD'
#define FOURCC_DXT3 '3TXD'
#define FOURCC_DXT5 '5TXD'

//struct
typedef struct _DDPixelFormat
{
	GLuint size;		//構造体のサイズ(32byte)
	GLuint flgas;		//サーフェスに含まれるデータの種類を表す
	GLuint fourCC;		//圧縮フォーマットを指定
	GLuint bpp;			//RGBAフォーマットのビット数
	GLuint redMask;		//各色に対応するマスクビット
	GLuint greenMask;
	GLuint blueMask;
	GLuint alphaMask;
} DDPixelFormat;

typedef struct _DDSCaps
{
	GLuint caps;	//DDSの能力を定義する(複合用に複数定義)
	GLuint caps2;
	GLuint caps3;
	GLuint caps4;
} DDSCaps;

typedef struct _DDColorKey
{
	GLuint lowVal;	//カラー キーとして使われる色の範囲の下限値 (下限値を含む)
	GLuint highVal;	//カラー キーとして使われる色の範囲の上限値 (上限値を含む)
} DDColorKey;

typedef struct _DDSurfaceDesc
{
	GLuint size;	//構造体のサイズ
	GLuint flags;	//有効なデータを含むメンバーを示す
	GLuint height;	//サーフェスの幅(ピクセル単位)
	GLuint width;	//サーフェスの高さ(ピクセル単位)
	GLuint pitch;	//一行のバイト数
	GLuint depth;	//3Dテクスチャの奥行
	GLuint mipMapLevels;	//ミップマップ数
	GLuint alphaBitDepth;	//アルファチャンネルのビット深度
	GLuint reserved;		//未使用
	GLuint surface;			//関連付けられたサーフェイスメモリへのポインタ

	DDColorKey ckDestOverlay;	//オーバーレイで使う転送先のカラーキー
	DDColorKey ckDestBlt;		//ブリットで使う転送先のカラーキー
	DDColorKey ckSrcOverlay;	//オーバーレイで使う転送元のカラーキー
	DDColorKey ckSrcBlt;		//ブリットで使う転送元のカラーキー

	DDPixelFormat format;		//DDPIXELFORMAT構造体
	DDSCaps caps;				//DDSCaps構造体

	GLuint textureStage;		//テクスチャをブレンドする時の順番
} DDSurfaceDesc;



//DDSImage class
class DDSImage
{
protected:
	GLuint imageSize;		//テクスチャのサイズ
	GLubyte *imageData;		//テクスチャのデータ
	GLenum format;			//圧縮フォーマット
	GLuint internalFormat;	//OpenGLが扱うテクスチャのフォーマットタイプ
	GLuint width;			//サーフェスの幅(ピクセル単位)
	GLuint height;			//サーフェスの高さ(ピクセル単位)
	GLuint bpp;				//RGBAフォーマットのビット数

	GLuint numMipmaps;		//ミップマップ数

public:
	GLuint ID;
	GLuint GetImageSize() { return imageSize; }
	GLubyte* GetImageData() { return imageData; }
	GLenum GetFormat() { return format; }
	GLuint GetInternalFormat() { return internalFormat; }
	GLuint GetWidth() { return width; }
	GLuint GetHeight() { return height; }
	GLuint GetBpp() { return bpp; }
	DDSImage();
	~DDSImage();
	void DecompressDDS();
	bool ReadDDS(const char *filename);
	GLuint Load(const char *filename);
};

#endif

