//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/12
//=====================================
#version 430

layout(triangles) in;	//�O�p�v���~�e�B�u���󂯎��
layout(triangle_strip, max_vertices = 3) out;	//in�Ŏ󂯎�������̂Ɠ����A3���_�̎O�p�`�X�g���b�v���o��

//�o�͕ϐ�
out vec3 normal;	//���_(�J����)���W�̖@��
out vec4 position;	//���_(�J����)���W�̈ʒu
noperspective out vec3 edgeDist;	//�O�p�`�̕ӂւ̋���(���`���)

//���͕ϐ�
in vec3	teNormal[];
in vec4 tePosition[];

uniform mat4 viewPortMatrix;	//�r���[�|�[�g�s��

void main()
{
	//���_�N���b�v���W(3����)�����ʋ�ԍ��W(2����)�֕ϊ�(w�ŏ��Z���Đ��m�ȃf�J���g���W�ɖ߂�)
	vec3 p0 = vec3(viewPortMatrix * (gl_in[0].gl_Position / 
									 gl_in[0].gl_Position.w));
	vec3 p1 = vec3(viewPortMatrix * (gl_in[1].gl_Position / 
									 gl_in[1].gl_Position.w));
	vec3 p2 = vec3(viewPortMatrix * (gl_in[2].gl_Position / 
									 gl_in[2].gl_Position.w));

	//����(ha, hb and hc)�����߂�
	float a = length(p1 - p2);
	float b = length(p2 - p0);
	float c = length(p1 - p0);

	//�]���@�����g���ē��p�𓾂�
	float alpha = acos((b*b + c*c - a*a) / (2.0*b*c));	
	float beta = acos((a*a + c*c - b*b) / (2.0*a*c));
	
	//�����𓾂�
	float ha = abs(c * sin(beta));	
	float hb = abs(c * sin(alpha));
	float hc = abs(b * sin(alpha));

	//�O�p�`�̊e���_�Ƃ��̑΂ɂȂ�ӂ̋����ƁA�e�l��ݒ肷�邱�Ƃ�
	//�������Ԃ��Ċe�t���O�����g����e�ӂւ̋�������������
	edgeDist = vec3(ha, 0, 0);
	normal = teNormal[0];
	position = tePosition[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();	//���_�ʒu�ƒ��_�P�ʂ̏o�͕ϐ��𔭍s

	edgeDist = vec3(0, hb, 0);
	normal = teNormal[1];
	position = tePosition[1];
	gl_Position = gl_in[1].gl_Position;
	EmitVertex();

	edgeDist = vec3(0, 0, hc);
	normal = teNormal[2];
	position = tePosition[2];
	gl_Position = gl_in[2].gl_Position;
	EmitVertex();

	EndPrimitive();	//�v���~�e�B�u�̂��ׂĂ̒��_�����s���ꂽ���Ƃ�ʒm
}