#version 330
const int MAX_LIGHTS = 4;

//Zmienne jednorodne
uniform mat4 P;
uniform mat4 V;
uniform mat4 M;

uniform vec4 lp[MAX_LIGHTS];//={vec4(20,0,0,1),vec4(-20,0,0,1)}; //trzeba jeszcze zrobiæ by to w funkcji przekazaæ

uniform vec4 lightDir=vec4(0,-1,0,0);

//Atrybuty
layout (location=0) in vec4 vertex; //wspolrzedne wierzcholka w przestrzeni modelu
layout (location=1) in vec4 normal; //wektor normalny w wierzcholku
layout (location=2) in vec2 texCoord; //wspó³rzêdne teksturowania


//Zmienne interpolowane
out vec2 i_tc;
//out float i_nl;
out vec4 l[MAX_LIGHTS];
out vec4 n;
out vec4 v;

void main(void) {


   // vec4 lightPos = vec4(0, 1, 10, 1);                   //pozycja œwiat³a
   // l = normalize(V * (lightPos - (V * M * vertex)));  //znormalizowany wektor do œwiat³a w przestrzeni oka

   for(int i=0; i<MAX_LIGHTS; i++)
   {
    l[i] = (V*lp[i]) - (V*M*vertex);                      //V decyduje o przypiêciu do kamery
   }
  
    n = normalize(V * M * normal);                       //znormalizowany wektor normalny w przestrzeni oka
    v = normalize(vec4(0, 0, 0, 1) - V * M * vertex);    //Wektor do obserwatora w przestrzeni oka
    
    gl_Position=P*V*M*vertex;

    mat4 G=mat4(inverse(transpose(mat3(M))));

   //vec4 n1=normalize(V*G*normal);
   //i_nl=clamp(dot(n1,lightDir),0,1);
    
    i_tc=texCoord;
}
