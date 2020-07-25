/*
Niniejszy program jest wolnym oprogramowaniem; możesz go
rozprowadzać dalej i / lub modyfikować na warunkach Powszechnej
Licencji Publicznej GNU, wydanej przez Fundację Wolnego
Oprogramowania - według wersji 2 tej Licencji lub(według twojego
wyboru) którejś z późniejszych wersji.

Niniejszy program rozpowszechniany jest z nadzieją, iż będzie on
użyteczny - jednak BEZ JAKIEJKOLWIEK GWARANCJI, nawet domyślnej
gwarancji PRZYDATNOŚCI HANDLOWEJ albo PRZYDATNOŚCI DO OKREŚLONYCH
ZASTOSOWAŃ.W celu uzyskania bliższych informacji sięgnij do
Powszechnej Licencji Publicznej GNU.

Z pewnością wraz z niniejszym programem otrzymałeś też egzemplarz
Powszechnej Licencji Publicznej GNU(GNU General Public License);
jeśli nie - napisz do Free Software Foundation, Inc., 59 Temple
Place, Fifth Floor, Boston, MA  02110 - 1301  USA
*/


#include <glm/glm.hpp>
#include <iostream>
#include <vector>
#include <fstream>
#include <string>

using namespace std;
using namespace glm;


vector<string> split(string line, char separator) {
	vector<string> ret;
	int pos = 0;
	while ((pos = line.find(separator)) != std::string::npos) {
		if (line.substr(0, pos).size() > 0)
			ret.push_back(line.substr(0, pos));
		line.erase(0, pos + 1);
	}
	ret.push_back(line);
	return ret;
}

vector<vec4> vtmp;
vector<vec2> vttmp;
vector<vec4> vntmp;
vector<vec4> ftmp;

vector<vec4> vertices;
vector<vec4> normals;
vector<vec2> texCoords;
int vertexCount;
void parse(vector<string> podzielona) {
	if (podzielona[0] == "v") {
		vtmp.push_back(vec4(stof(podzielona[1]), stof(podzielona[2]), stof(podzielona[3]), 1.0f));
	}
	else if (podzielona[0] == "vn") {
		vntmp.push_back(vec4(stof(podzielona[1]), stof(podzielona[2]), stof(podzielona[3]), 0));
	}
	else if (podzielona[0] == "vt") {
		vttmp.push_back(vec2(stof(podzielona[1]), stof(podzielona[2])));
	}
	else if (podzielona[0] == "f") {
		for (int i = 1; i < 4; i++) {
			vector<string> faces;
			faces = split(podzielona[i], '/');
			vertices.push_back(vtmp[stoi(faces[0]) - 1]);
			texCoords.push_back(vttmp[stoi(faces[1]) - 1]);
			normals.push_back(vntmp[stoi(faces[2]) - 1]);
		}
	}
}

void readfile(string name) {
	string line;
	ifstream infile(name, ios_base::in);
	while (getline(infile, line, '\n')) {
		parse(split(line, ' '));
	}
	vertexCount = vertices.size();
	infile.close();
}



int main(int argc, char* argv[])
{
	cout << "nazwa: ";
	string nazwa;
	cin >> nazwa;
	readfile(string("models/") + nazwa);
	nazwa = nazwa.substr(0, nazwa.length() - 4);
	ofstream outFile(string(string("models/").append(nazwa)).append(string(".h")), ofstream::out | ofstream::trunc);
	outFile << "float " << nazwa << "Vertices [] = { ";
	for (int i = 0; i < vertices.size() - 1; i++) {
		for (int j = 0; j < 4; j++)
		{
			outFile << vertices[i][j] << ", ";
		}
		outFile << endl;
	}
	for (int j = 0; j < 3; j++)
	{
		outFile << vertices[vertices.size() - 1][j] << ", ";
	}
	outFile << vertices[vertices.size() - 1][3] << "};" << endl << endl;


	outFile << "float " << nazwa << "Normals [] = { ";
	for (int i = 0; i < normals.size() - 1; i++) {
		for (int j = 0; j < 4; j++)
		{
			outFile << normals[i][j] << ", ";
		}
		outFile << endl;
	}
	for (int j = 0; j < 3; j++)
	{
		outFile << normals[normals.size() - 1][j] << ", ";
	}
	outFile << normals[normals.size() - 1][3] << "};" << endl << endl;

	outFile << "float " << nazwa << "TexCoords[] = { ";
	for (int i = 0; i < texCoords.size() - 1; i++) {
		for (int j = 0; j < 2; j++)
		{
			outFile << texCoords[i][j] << ", ";
		}
		outFile << endl;
	}
	outFile << texCoords[texCoords.size() - 1][0] << ", ";
	outFile << texCoords[texCoords.size() - 1][1] << "};" << endl << endl;

	outFile << "int " << nazwa << "VertexCount = " << vertexCount << ";\n";
}