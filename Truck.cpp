#include "Truck.h"

struct _vec2 { float x; float y; };
typedef struct _vec2 vec2;

int start(vec2* vertices1, vec2* vertices2);

Truck::Truck(Model_type model_type, glm::vec3 transform, glm::vec3 scale, std::string texture_name, float rotation_angle, glm::vec3 rotation, bool is_visible, bool is_rotating)
	:Basic(model_type, transform, scale, texture_name, rotation_angle, rotation, is_visible, is_rotating) {
	car_speed = 0.0f;
	steer_angle = 0.0f;
	can_move = true;
	acceleration = 0.0f;
	friction = 0.2f;
	accelerating_backwards = false;
	accelerating_forwards = false;
}

// Trzeba to zrobic inaczej!
void Truck::movement() {
	// Obliczenia zwiazane z przyspieszeniem i predkoscia
	if (acceleration > 0.0f && accelerating_backwards == false) {
		accelerating_forwards = true;
	}
	if (acceleration < 0.0f && accelerating_forwards == false) {
		accelerating_backwards = true;
	}
	if (accelerating_forwards)
	{
		this->car_speed += acceleration;
		this->car_speed -= friction;
		if (this->car_speed > 20.0f) {
			this->car_speed = 20.0f;
		}
	}
	if (accelerating_backwards)
	{
		this->car_speed += -acceleration;
		this->car_speed -= friction;
		if (this->car_speed > 20.0f) {
			this->car_speed = 20.0f;
		}
	}
	if (accelerating_forwards) {
		if (this->car_speed <= 0.0f) {
			accelerating_forwards = false;
			this->car_speed = 0.0f;
		}
	}
	if (accelerating_backwards) {
		if (this->car_speed <= 0.0f) {
			accelerating_backwards = false;
			this->car_speed = 0.0f;
		}
	}
	// Pozycje przednia i tylna w przestrzenii modelu
	glm::vec4 temp1 = getMMatrix() * glm::vec4(0.0f, 0.0f, -50.0f, 1.0f);
	glm::vec4 temp2 = getMMatrix() * glm::vec4(0.0f, 0.0f, 50.0f, 1.0f);
	glm::vec3 punkt_przedni = glm::vec3(temp1.x, temp1.y, temp1.z);
	glm::vec3 punkt_tylny = glm::vec3(temp2.x, temp2.y, temp2.z);

	float speed_x;
	float speed_z;
	float speed_x_steer;
	float speed_z_steer;
	// Obliczamy wektory predkosci
	if (accelerating_backwards) {
		speed_x = glm::sin(glm::radians(this->rotation_angle)) * -this->car_speed * float(glfwGetTime());
		speed_z = glm::cos(glm::radians(this->rotation_angle)) * -this->car_speed * float(glfwGetTime());
		speed_x_steer = glm::sin(glm::radians(this->rotation_angle + this->steer_angle)) * -this->car_speed * float(glfwGetTime());
		speed_z_steer = glm::cos(glm::radians(this->rotation_angle + this->steer_angle)) * -this->car_speed * float(glfwGetTime());
	}
	else {
		speed_x = glm::sin(glm::radians(this->rotation_angle)) * this->car_speed * float(glfwGetTime());
		speed_z = glm::cos(glm::radians(this->rotation_angle)) * this->car_speed * float(glfwGetTime());
		speed_x_steer = glm::sin(glm::radians(this->rotation_angle + this->steer_angle)) * this->car_speed * float(glfwGetTime());
		speed_z_steer = glm::cos(glm::radians(this->rotation_angle + this->steer_angle)) * this->car_speed * float(glfwGetTime());
	}
	// Obliczenie nowych pozycji punktow
	glm::vec3 punkt_przedni_new = punkt_przedni + glm::vec3(speed_x_steer, 0.0f, speed_z_steer);
	glm::vec3 punkt_tylny_new = punkt_tylny + glm::vec3(speed_x, 0.0f, speed_z);

	// Obliczamy nowy punkt srodkowy (wspolrzedna modelu)
	glm::vec3 new_pos = glm::vec3(punkt_przedni_new.x + punkt_tylny_new.x, punkt_przedni_new.y + punkt_tylny_new.y, punkt_przedni_new.z + punkt_tylny_new.z) / 2.0f;
	this->translate(new_pos);
	
	// Obliczamy nowy kat
	this->rotation_angle = 180.0f*(glm::atan(punkt_tylny_new.x - punkt_przedni_new.x, punkt_tylny_new.z - punkt_przedni_new.z))/PI;
	//std::cout << "Position: x: " << new_pos.x << " y: " << new_pos.y << " z: " << new_pos.z << std::endl;
	//std::cout << "Rotation angle: " << this->rotation_angle << std::endl;
}

bool Truck::collision(Basic* other) {
	glm::mat4 otherM = other->getMMatrix();
	glm::mat4 truckM = this->getMMatrix();
	glm::vec4 minTruckVec = truckM * glm::vec4(this->min_vert_x, 0.0f, this->min_vert_z, 1.0f);
	//std::cout << "Min truck x:" << minTruckVec.x << " z:" << minTruckVec.z  << std::endl;
	glm::vec4 maxTruckVec = truckM * glm::vec4(this->max_vert_x, 0.0f, this->max_vert_z, 1.0f);
	//std::cout << "Max truck x:" << maxTruckVec.x << " z:" << maxTruckVec.z << std::endl;
	glm::vec4 minOtherVec = otherM * glm::vec4(other->min_vert_x, 0.0f, other->min_vert_z, 1.0f);
	//std::cout << "Min other x:" << minOtherVec.x << " z:" << minOtherVec.z << std::endl;
	glm::vec4 maxOtherVec = otherM * glm::vec4(other->max_vert_x, 0.0f, other->max_vert_z, 1.0f);
	//std::cout << "Max other x:" << maxOtherVec.x << " z:" << maxOtherVec.z << std::endl;
	if (minTruckVec.x > maxTruckVec.x) {
		std::swap(minTruckVec.x, maxTruckVec.x);
	}
	if (minTruckVec.z > maxTruckVec.z) {
		std::swap(minTruckVec.z, maxTruckVec.z);
	}
	if (minOtherVec.x > maxOtherVec.x) {
		std::swap(minOtherVec.x, maxOtherVec.x);
	}
	if (minOtherVec.z > maxOtherVec.z) {
		std::swap(minOtherVec.z, maxOtherVec.z);
	}
	//std::cout << "X: " << minOtherVec.x << " " <<  maxOtherVec.x << " T: "<< minTruckVec.x << " " << maxTruckVec.x << std::endl;
	//std::cout << "Z: " << minOtherVec.z << " " <<  maxOtherVec.z << " T: "<< minTruckVec.z << " " << maxTruckVec.z << std::endl;
	// Przygotowanie danych do biblioteki sprawdzania kolizji
	vec2 vertices1[4];
	vec2 vertices2[4];

	
	vertices1[0].x = minTruckVec.x;
	vertices1[1].x = minTruckVec.x;
	vertices1[2].x = maxTruckVec.x;
	vertices1[3].x = maxTruckVec.x;
	vertices1[0].y = minTruckVec.z;
	vertices1[1].y = maxTruckVec.z;
	vertices1[2].y = maxTruckVec.z;
	vertices1[3].y = minTruckVec.z;
	vertices2[0].x = minOtherVec.x; 
	vertices2[1].x = minOtherVec.x;
	vertices2[2].x = maxOtherVec.x;
	vertices2[3].x = maxOtherVec.x;
	vertices2[0].y = minOtherVec.z;
	vertices2[1].y = maxOtherVec.z;
	vertices2[2].y = maxOtherVec.z;
	vertices2[3].y = minOtherVec.z;
	if (start(vertices1, vertices2) == 1) {
		return true;
	}
	else
		return false;
}

void Truck::change_angle(float value) {
	this->steer_angle += value;
	//std::cout << "Steer angle: " << steer_angle << std::endl;
	if (this->steer_angle > 90.0f)
		this->steer_angle = 90.0f;
	if (this->steer_angle < -90.0f)
		this->steer_angle = -90.0f;
}

//-----------------------------------------------------------------------------
// Gilbert-Johnson-Keerthi (GJK) collision detection algorithm in 2D
// http://www.dyn4j.org/2010/04/gjk-gilbert-johnson-keerthi/
// http://mollyrocket.com/849
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
// Basic vector arithmetic operations

vec2 subtract(vec2 a, vec2 b) { a.x -= b.x; a.y -= b.y; return a; }
vec2 negate(vec2 v) { v.x = -v.x; v.y = -v.y; return v; }
vec2 perpendicular(vec2 v) { vec2 p = { v.y, -v.x }; return p; }
float dotProduct(vec2 a, vec2 b) { return a.x * b.x + a.y * b.y; }
float lengthSquared(vec2 v) { return v.x * v.x + v.y * v.y; }

//-----------------------------------------------------------------------------
// Triple product expansion is used to calculate perpendicular normal vectors 
// which kinda 'prefer' pointing towards the Origin in Minkowski space

vec2 tripleProduct(vec2 a, vec2 b, vec2 c) {

	vec2 r;

	float ac = a.x * c.x + a.y * c.y; // perform a.dot(c)
	float bc = b.x * c.x + b.y * c.y; // perform b.dot(c)

	// perform b * a.dot(c) - a * b.dot(c)
	r.x = b.x * ac - a.x * bc;
	r.y = b.y * ac - a.y * bc;
	return r;
}

//-----------------------------------------------------------------------------
// This is to compute average center (roughly). It might be different from
// Center of Gravity, especially for bodies with nonuniform density,
// but this is ok as initial direction of simplex search in GJK.

vec2 averagePoint(const vec2* vertices, size_t count) {
	vec2 avg = { 0.f, 0.f };
	for (size_t i = 0; i < count; i++) {
		avg.x += vertices[i].x;
		avg.y += vertices[i].y;
	}
	avg.x /= count;
	avg.y /= count;
	return avg;
}

//-----------------------------------------------------------------------------
// Get furthest vertex along a certain direction

size_t indexOfFurthestPoint(const vec2* vertices, size_t count, vec2 d) {

	float maxProduct = dotProduct(d, vertices[0]);
	size_t index = 0;
	for (size_t i = 1; i < count; i++) {
		float product = dotProduct(d, vertices[i]);
		if (product > maxProduct) {
			maxProduct = product;
			index = i;
		}
	}
	return index;
}

//-----------------------------------------------------------------------------
// Minkowski sum support function for GJK

vec2 support(const vec2* vertices1, size_t count1,
	const vec2* vertices2, size_t count2, vec2 d) {

	// get furthest point of first body along an arbitrary direction
	size_t i = indexOfFurthestPoint(vertices1, count1, d);

	// get furthest point of second body along the opposite direction
	size_t j = indexOfFurthestPoint(vertices2, count2, negate(d));

	// subtract (Minkowski sum) the two points to see if bodies 'overlap'
	return subtract(vertices1[i], vertices2[j]);
}

//-----------------------------------------------------------------------------
// The GJK yes/no test

int iter_count = 0;

int gjk(const vec2* vertices1, size_t count1,
	const vec2* vertices2, size_t count2) {

	size_t index = 0; // index of current vertex of simplex
	vec2 a, b, c, d, ao, ab, ac, abperp, acperp, simplex[3];

	vec2 position1 = averagePoint(vertices1, count1); // not a CoG but
	vec2 position2 = averagePoint(vertices2, count2); // it's ok for GJK )

	// initial direction from the center of 1st body to the center of 2nd body
	d = subtract(position1, position2);

	// if initial direction is zero – set it to any arbitrary axis (we choose X)
	if ((d.x == 0) && (d.y == 0))
		d.x = 1.f;

	// set the first support as initial point of the new simplex
	a = simplex[0] = support(vertices1, count1, vertices2, count2, d);

	if (dotProduct(a, d) <= 0)
		return 0; // no collision

	d = negate(a); // The next search direction is always towards the origin, so the next search direction is negate(a)

	while (1) {
		iter_count++;

		a = simplex[++index] = support(vertices1, count1, vertices2, count2, d);

		if (dotProduct(a, d) <= 0)
			return 0; // no collision

		ao = negate(a); // from point A to Origin is just negative A

		// simplex has 2 points (a line segment, not a triangle yet)
		if (index < 2) {
			b = simplex[0];
			ab = subtract(b, a); // from point A to B
			d = tripleProduct(ab, ao, ab); // normal to AB towards Origin
			if (lengthSquared(d) == 0)
				d = perpendicular(ab);
			continue; // skip to next iteration
		}

		b = simplex[1];
		c = simplex[0];
		ab = subtract(b, a); // from point A to B
		ac = subtract(c, a); // from point A to C

		acperp = tripleProduct(ab, ac, ac);

		if (dotProduct(acperp, ao) >= 0) {

			d = acperp; // new direction is normal to AC towards Origin

		}
		else {

			abperp = tripleProduct(ac, ab, ab);

			if (dotProduct(abperp, ao) < 0)
				return 1; // collision

			simplex[0] = simplex[1]; // swap first element (point C)

			d = abperp; // new direction is normal to AB towards Origin
		}

		simplex[1] = simplex[2]; // swap element in the middle (point B)
		--index;
	}

	return 0;
}

//-----------------------------------------------------------------------------

#include <stdlib.h>
#include <float.h>

float Perturbation()
{
	return ((float)rand() / (float)RAND_MAX) * FLT_EPSILON * 100.0f * ((rand() % 2) ? 1.0f : -1.0f);
}

vec2 Jostle(vec2 a)
{
	vec2 b;
	b.x = a.x + Perturbation();
	b.y = a.y + Perturbation();
	return b;
}

int start(vec2* vertices1, vec2* vertices2) {
	size_t count1 = 4;
	size_t count2 = 4;
	//std::cout << count1 << " " << count2 << std::endl;
	while (1)
	{
		vec2 a[4];
		vec2 b[4];

		for (size_t i = 0; i < count1; ++i) a[i] = Jostle(vertices1[i]);
		for (size_t i = 0; i < count2; ++i) b[i] = Jostle(vertices2[i]);

		int collisionDetected = gjk(a, count1, b, count2);
		if (!collisionDetected)
		{
			//std::cout << "NO collision!" << std::endl;
			return 0;
		}
		else
		{
			std::cout << "Collision!" << std::endl;
			printf("Case:\n\t{%f, %f}, {%f, %f}, {%f, %f}, {%f, %f}\n\t{%f, %f}, {%f, %f}, {%f, %f}, {%f, %f}\n\n",
				a[0].x, a[0].y, a[1].x, a[1].y, a[2].x, a[2].y, a[3].x, a[3].y,
				b[0].x, b[0].y, b[1].x, b[1].y, b[2].x, b[2].y, b[3].x, b[3].y
			);
			return 1;
		}
		iter_count = 0;
	}

	return 0;
}
