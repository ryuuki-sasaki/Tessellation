//=====================================
//制作者：佐々木隆貴
//日付：2016/5/12
//=====================================
#version 430

struct lightInfo
{
	vec4 position;	//頂点座標でのライト位置
	vec3 intensity;		//ADS　ライト強度
};
uniform lightInfo light;

//メッシュの線の設定
uniform struct lineInfo
{
	float width;	//メッシュの線の幅の半分
	vec4 color;		//メッシュの線の色
}line;

uniform vec3 kd;

//入力変数
in vec4 position;
in vec3 normal;
noperspective in vec3 edgeDist;		//線形補間
 
//出力変数
layout(location=0)out vec4 flagColor;

//=====================================
//光の強度を求める
//position:頂点位置
//norm:頂点法線
//=====================================
vec3 DiffuseModel( vec3 position, vec3 norm )
{
	vec3 s = normalize(vec3(light.position - position));	//光源に対する正規化方向

	float sDotN = max(dot(s, norm), 0.0);	//正規化されたベクトルの内積をとる(dotで0以下(角度90度を超える)の値を値をとらないようにする)
	vec3 diffuse = light.intensity * kd * sDotN;	//ディフューズ成分計算

	return diffuse;
}

//=====================================
//線の色との混合率を決定
//=====================================
float edgeMix()
{
	//最小の距離を求める
	float d = min(edgeDist.x, edgeDist.y);
	d = min(d, edgeDist.z);
	
	//線の色との混合率を決定(line.width - 1未満であれば0、line.width + 1より大きければ1を返す)
	float mixVal = smoothstep(line.width - 1,
							 	line.width + 1, d);

	return mixVal;
}

void main()
{
	float mixVal = edgeMix();
	vec4 color = vec4(DiffuseModel(position.xyz, normal), 1.0);

	//サーフェイスの色と線の色を混ぜる
	flagColor = mix(color, line.color, mixVal);
}