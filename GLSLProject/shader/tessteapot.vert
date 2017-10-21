//=====================================
//制作者：佐々木隆貴
//日付：2016/5/12
//=====================================
#version 430
 
//入力変数
layout (location=0) in vec3 vertexPosition;	
layout (location=1) in vec3 vertexNormal;

//出力変数
out vec3 normal;

void main()
{
	//法線を渡す
	normal = vertexNormal;
	//位置を後に渡す
	gl_Position = vec4(vertexPosition, 1.0);
}
