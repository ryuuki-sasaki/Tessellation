//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/12
//=====================================
#version 430

struct lightInfo
{
	vec4 position;	//���_���W�ł̃��C�g�ʒu
	vec3 intensity;		//ADS�@���C�g���x
};
uniform lightInfo light;

//���b�V���̐��̐ݒ�
uniform struct lineInfo
{
	float width;	//���b�V���̐��̕��̔���
	vec4 color;		//���b�V���̐��̐F
}line;

uniform vec3 kd;

//���͕ϐ�
in vec4 position;
in vec3 normal;
noperspective in vec3 edgeDist;		//���`���
 
//�o�͕ϐ�
layout(location=0)out vec4 flagColor;

//=====================================
//���̋��x�����߂�
//position:���_�ʒu
//norm:���_�@��
//=====================================
vec3 DiffuseModel( vec3 position, vec3 norm )
{
	vec3 s = normalize(vec3(light.position - position));	//�����ɑ΂��鐳�K������

	float sDotN = max(dot(s, norm), 0.0);	//���K�����ꂽ�x�N�g���̓��ς��Ƃ�(dot��0�ȉ�(�p�x90�x�𒴂���)�̒l��l���Ƃ�Ȃ��悤�ɂ���)
	vec3 diffuse = light.intensity * kd * sDotN;	//�f�B�t���[�Y�����v�Z

	return diffuse;
}

//=====================================
//���̐F�Ƃ̍�����������
//=====================================
float edgeMix()
{
	//�ŏ��̋��������߂�
	float d = min(edgeDist.x, edgeDist.y);
	d = min(d, edgeDist.z);
	
	//���̐F�Ƃ̍�����������(line.width - 1�����ł����0�Aline.width + 1���傫�����1��Ԃ�)
	float mixVal = smoothstep(line.width - 1,
							 	line.width + 1, d);

	return mixVal;
}

void main()
{
	float mixVal = edgeMix();
	vec4 color = vec4(DiffuseModel(position.xyz, normal), 1.0);

	//�T�[�t�F�C�X�̐F�Ɛ��̐F��������
	flagColor = mix(color, line.color, mixVal);
}