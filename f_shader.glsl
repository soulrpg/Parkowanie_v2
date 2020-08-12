#version 330
const int MAX_LIGHTS = 3;
const float shineDumper = 7;
const float reflectivity = 0.3;
const vec4 lightColour = vec4(1,1,1,1);

uniform sampler2D tex;

out vec4 pixelColor; //Zmienna wyjsciowa fragment shadera. Zapisuje sie do niej ostateczny (prawie) kolor piksela

//Zmienne interpolowane
//in float i_nl;
in vec2 i_tc;

in vec4 l[MAX_LIGHTS];
in vec4 n;
in vec4 v;

void main(void) {
	
	vec4 mn = normalize(n);
	vec4 mv = normalize(v);

	vec4 totalDiffuse = vec4(0.0);
	vec4 totalSpecular = vec4(0.0);

	for(int i=0; i<MAX_LIGHTS; i++)
	{
		vec4 ml = normalize(l[i]);
		float nDotl= dot(ml,mn);
		float brightness = max(nDotl,0.0);

		vec4 lightDirection= -ml;
		vec4 reflectedLightDirection = reflect(lightDirection,mn);

		float specularFactor = dot(reflectedLightDirection, mv);
		specularFactor = max(specularFactor,0.0);
		float dumpedFactor = pow(specularFactor,shineDumper);
		totalDiffuse += brightness*lightColour;
		totalSpecular += dumpedFactor * reflectivity * lightColour;
	}
	totalDiffuse= max(totalDiffuse,0.2);

	vec4 color=texture(tex,i_tc);
	pixelColor = (totalDiffuse*color)+totalSpecular; //vec4(color.rgb*i_nl,color.a)=color
}
