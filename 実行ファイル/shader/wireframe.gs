//=====================================
//制作者：佐々木隆貴
//日付：2016/5/12
//=====================================
#version 430

layout(triangles) in;	//三角プリミティブを受け取る
layout(triangle_strip, max_vertices = 3) out;	//inで受け取ったものと同じ、3頂点の三角形ストリップを出力

//出力変数
out vec3 normal;	//視点(カメラ)座標の法線
out vec4 position;	//視点(カメラ)座標の位置
noperspective out vec3 edgeDist;	//三角形の辺への距離(線形補間)

//入力変数
in vec3	teNormal[];
in vec4 tePosition[];

uniform mat4 viewPortMatrix;	//ビューポート行列

void main()
{
	//頂点クリップ座標から画面空間座標へ変換(wで除算して正確なデカルト座標に戻す)
	vec3 p0 = vec3(viewPortMatrix * (gl_in[0].gl_Position / 
									 gl_in[0].gl_Position.w));
	vec3 p1 = vec3(viewPortMatrix * (gl_in[1].gl_Position / 
									 gl_in[1].gl_Position.w));
	vec3 p2 = vec3(viewPortMatrix * (gl_in[2].gl_Position / 
									 gl_in[2].gl_Position.w));

	//高さ(ha, hb and hc)を求める
	float a = length(p1 - p2);
	float b = length(p2 - p0);
	float c = length(p1 - p0);

	//余弦法則を使って内角を得る
	float alpha = acos((b*b + c*c - a*a) / (2.0*b*c));	
	float beta = acos((a*a + c*c - b*b) / (2.0*a*c));
	
	//高さを得る
	float ha = abs(c * sin(beta));	
	float hb = abs(c * sin(alpha));
	float hc = abs(b * sin(alpha));

	//三角形の各頂点とその対になる辺の距離と、各値を設定することで
	//それらを補間して各フラグメントから各辺への距離を供給する
	edgeDist = vec3(ha, 0, 0);
	normal = teNormal[0];
	position = tePosition[0];
	gl_Position = gl_in[0].gl_Position;
	EmitVertex();	//頂点位置と頂点単位の出力変数を発行

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

	EndPrimitive();	//プリミティブのすべての頂点が発行されたことを通知
}