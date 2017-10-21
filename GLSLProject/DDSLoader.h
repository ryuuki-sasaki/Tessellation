//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/3/30
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
	GLuint size;		//�\���̂̃T�C�Y(32byte)
	GLuint flgas;		//�T�[�t�F�X�Ɋ܂܂��f�[�^�̎�ނ�\��
	GLuint fourCC;		//���k�t�H�[�}�b�g���w��
	GLuint bpp;			//RGBA�t�H�[�}�b�g�̃r�b�g��
	GLuint redMask;		//�e�F�ɑΉ�����}�X�N�r�b�g
	GLuint greenMask;
	GLuint blueMask;
	GLuint alphaMask;
} DDPixelFormat;

typedef struct _DDSCaps
{
	GLuint caps;	//DDS�̔\�͂��`����(�����p�ɕ�����`)
	GLuint caps2;
	GLuint caps3;
	GLuint caps4;
} DDSCaps;

typedef struct _DDColorKey
{
	GLuint lowVal;	//�J���[ �L�[�Ƃ��Ďg����F�͈̔͂̉����l (�����l���܂�)
	GLuint highVal;	//�J���[ �L�[�Ƃ��Ďg����F�͈̔͂̏���l (����l���܂�)
} DDColorKey;

typedef struct _DDSurfaceDesc
{
	GLuint size;	//�\���̂̃T�C�Y
	GLuint flags;	//�L���ȃf�[�^���܂ރ����o�[������
	GLuint height;	//�T�[�t�F�X�̕�(�s�N�Z���P��)
	GLuint width;	//�T�[�t�F�X�̍���(�s�N�Z���P��)
	GLuint pitch;	//��s�̃o�C�g��
	GLuint depth;	//3D�e�N�X�`���̉��s
	GLuint mipMapLevels;	//�~�b�v�}�b�v��
	GLuint alphaBitDepth;	//�A���t�@�`�����l���̃r�b�g�[�x
	GLuint reserved;		//���g�p
	GLuint surface;			//�֘A�t����ꂽ�T�[�t�F�C�X�������ւ̃|�C���^

	DDColorKey ckDestOverlay;	//�I�[�o�[���C�Ŏg���]����̃J���[�L�[
	DDColorKey ckDestBlt;		//�u���b�g�Ŏg���]����̃J���[�L�[
	DDColorKey ckSrcOverlay;	//�I�[�o�[���C�Ŏg���]�����̃J���[�L�[
	DDColorKey ckSrcBlt;		//�u���b�g�Ŏg���]�����̃J���[�L�[

	DDPixelFormat format;		//DDPIXELFORMAT�\����
	DDSCaps caps;				//DDSCaps�\����

	GLuint textureStage;		//�e�N�X�`�����u�����h���鎞�̏���
} DDSurfaceDesc;



//DDSImage class
class DDSImage
{
protected:
	GLuint imageSize;		//�e�N�X�`���̃T�C�Y
	GLubyte *imageData;		//�e�N�X�`���̃f�[�^
	GLenum format;			//���k�t�H�[�}�b�g
	GLuint internalFormat;	//OpenGL�������e�N�X�`���̃t�H�[�}�b�g�^�C�v
	GLuint width;			//�T�[�t�F�X�̕�(�s�N�Z���P��)
	GLuint height;			//�T�[�t�F�X�̍���(�s�N�Z���P��)
	GLuint bpp;				//RGBA�t�H�[�}�b�g�̃r�b�g��

	GLuint numMipmaps;		//�~�b�v�}�b�v��

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

