//======================================
//	�Q�l�}��:OpenGL4.0�V�F�[�f�B���O����
//	�t���R�[�h�g�p
//	�����:David Wolff.
//======================================
#ifndef VBOTEAPOTPATCH_H
#define VBOTEAPOTPATCH_H

#include <glm/glm/glm.hpp>
using glm::vec3;
using glm::mat3;
using glm::mat4;

class VBOTeapotPatch
{
private:
	unsigned int vaoHandle;
	float * n;

	void generatePatches(float * v, float *n);
	void buildPatchReflect(int patchNum,
		float *v, float *n, int &index,
		bool reflectX, bool reflectY);
	void buildPatch(vec3 patch[][4],
		float *v, float * n, int &index, mat3 reflect);
	void getPatch(int patchNum, vec3 patch[][4], bool reverseV);

public:
	VBOTeapotPatch();

	void render() const;
};

#endif // VBOTEAPOTPATCH_H
