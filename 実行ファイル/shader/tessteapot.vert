//=====================================
//����ҁF���X�ؗ��M
//���t�F2016/5/12
//=====================================
#version 430
 
//���͕ϐ�
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;

//�o�͕ϐ�
out vec3 normal;

void main()
{
	//�@����n��
	normal = vertexNormal;
	//�ʒu����ɓn��
	gl_Position = vec4(vertexPosition, 1.0);
}
