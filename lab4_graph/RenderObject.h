#pragma once

#pragma once
#include "framework.h"

#include <fstream>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <list>
#include <vector>

const float PI = 3.141592653589793238462643383279502884L;             //константы, говорят сами за себя

const int ScreenHeigth = 900;
const int ScreenWidth = 1600;


using namespace std;




namespace Allocator       // пространство имён аллокатора памяти
{
	void** create_matrix(int heigth, int width, int size_elem)             //выделить память под двумерный массив чего нибудь 
	{
		void** arr = (void**)malloc(sizeof(void*) * heigth);

		for (int i = 0; i < heigth; i++)
		{
			arr[i] = malloc(size_elem * width);
		}

		return arr;
	}

	void delete_arr(void** arr, int heigth)             //удалить двумерный массив, выделенный таким образом
	{
		for (int i = 0; i < heigth; i++)
		{
			free(arr[i]);
		}
		free((void*)arr);
	}
};

namespace Product             //пространство имён умножателя?
{
	inline float** multiply_matrix(float arr1[][3], int h1, int w1, float arr2[][3], int h2, int w2) {     //умножаем 2 матрицы 3х3

		float** res = reinterpret_cast<float**>(Allocator::create_matrix(h1, w2, sizeof(float)));

		if (h1 == w2 || h2 == w1) {
			for (int i = 0; i < h1; i++) {


				for (int j = 0; j < w1; j++) {
					float* current_row = arr1[i];
					float sum = 0;
					for (int k = 0; k < h2; k++) {
						sum += current_row[k] * arr2[k][j];
					}
					res[i][j] = sum;
				}
			}
		}

		return res;
	}

	inline float** multiply_matrix(float arr1[][4], int h1, int w1, float arr2[][4], int h2, int w2) {    //умножаем 2 матрицы 4х4

		float** res = reinterpret_cast<float**>(Allocator::create_matrix(h1, w2, sizeof(float)));    //реинтерпретируем void** того что нам вернул аллокатор к float** (можно было по другому сделать, но так тожн норм)

		if (h1 == w2 || h2 == w1) {
			for (int i = 0; i < h1; i++) {


				for (int j = 0; j < w1; j++) {
					float* current_row = arr1[i];
					float sum = 0;
					for (int k = 0; k < h2; k++) {
						sum += current_row[k] * arr2[k][j];
					}
					res[i][j] = sum;
				}
			}
		}

		return res;
	}

}

class ViewPoint //класс точки обзора 
{
public:

	float O_hor;
	float fi_vert;

	float R;

	float focus_distance = 0;

	ViewPoint(float v, float h, float dist, float focus) :fi_vert(v* PI / 180), O_hor(h* PI / 180), R(dist), focus_distance(focus)
	{
		cout << O_hor << endl;
		cout << fi_vert << endl;
		cout << R << endl;
	};
};

class Point2D   // класс двумерной точки
{
public:
	float x = 0;
	float y = 0;

	Point2D() {};

	Point2D(float a, float b) : x(a), y(b)
	{

	}
};

void showPoint(Point2D point);

class Point3D : public Point2D // класс трёжмерной точки ( наследует 2-мерную точку, можно было сделать композицией, но...)
{
public:
	float z = 0;

	Point3D() {}

	Point3D(float a, float b, float c) : Point2D::Point2D(a, b), z(c)
	{

	}

	static inline Point3D* goToWorldCoordinates(float view_matrix[4][4], Point3D points[], int size) //статик метода перехода множества точек к новой системе координат (тупо матан)
	{
		Point3D* arr = new Point3D[size];

		for (int i = 0; i < size; i++)
		{
			float point_matrix[1][4] = { {points[i].x, points[i].y, points[i].z, 1} };

			float** product = Product::multiply_matrix(point_matrix, 1, 4, view_matrix, 4, 4);

			Point3D res(0, 0, 0);
			res.x = product[0][0];
			res.y = product[0][1];
			res.z = product[0][2];

			arr[i] = res;

			Allocator::delete_arr((void**)product, 1);
		}

		return arr;
	}

	static inline Point2D* perspectiveTransformation(ViewPoint* camera, Point3D* objects, int size)      //перспективное преобразование массива трёхмерных точек - на выходе массив двумерных координат
	{
		float transform_matrix[4][4] = {
			{1,0,0,0},
			{0,1,0,0},
			{0,0,1,camera->focus_distance},
			{0,0,0,1}
		};

		Point2D* arr = new Point2D[size];
		for (int i = 0; i < size; i++)
		{
			float coord_matrix[1][4] = { {objects[i].x ,objects[i].y, objects[i].z, 1.0} };      //опять матан, с аллокаторами, но всё же матан


			float** res = Product::multiply_matrix(coord_matrix, 1, 4, transform_matrix, 4, 4);

			res[0][0] /= res[0][3];
			res[0][1] /= res[0][3];

			auto temp = Point2D(res[0][0], res[0][1]);

			arr[i] = temp;

			Allocator::delete_arr((void**)res, 1);
		}

		return arr;
	}

	static Point2D displayTransformation(Point2D p, int heigth, int width) // юзлес метод, ненада
	{

	}
};
void showPoint(Point2D point);

Point3D getDecartCoord(ViewPoint p) //тоже ненада
{
	return Point3D(p.R * sin(p.fi_vert) * cos(p.O_hor), p.R * sin(p.O_hor) * sin(p.fi_vert), p.R * cos(p.fi_vert));
}

void showPoint(Point3D point) // also
{
	cout << point.x << "  " << point.y << "  " << point.z << endl;
}

void print_arr(float** arr, int h, int w)// also
{
	for (int i = 0; i < h; i++) {
		for (int j = 0; j < w; j++)
		{
			cout << arr[i][j] << " ";
		}
		cout << endl;
	}
}

enum AxisDirection { X, Y, Z };



class _Polygon       //во, фигура (трёхмерная)
{
private:
	Point3D* points;
	int size;

	friend class RenderableObject;     //дружественный класс тот, который будет нас рендерить, то есть проецировать
	friend class Transform;

public:

	_Polygon() {}

	_Polygon(Point3D* arr, int s)     //конструктор, где это инициализируется
	{
		size = s;
		points = arr;

		/*for (int i = 0; i < size; i++)
		{
			points[i] = res[i];
		}*/
	}

	Point2D* render(ViewPoint* camera, float cam_matrix[4][4])       //проецируем
	{

		auto temp = Point3D::goToWorldCoordinates(cam_matrix, this->points, size);     // сначала переносим точки в новую систему координат
		Point2D* res = Point3D::perspectiveTransformation(camera,
			temp, size);      //проецируем

		delete[] temp;
		/*for (int i = 0; i < size; i++)
		{
			showPoint(res[i]);
		}*/

		return res;  //возаращаем результат
	}

	~_Polygon()//ненужный деструктор
	{
		//delete[] points;
	}

};



class RenderedPolygon
{
public:
	int size;

	Point2D* arr;

	RenderedPolygon(Point2D* res, int s) : size(s)
	{
		arr = res;

		/*for (int i = 0; i < size; i++)
		{
			res[i] = res[i];
		}*/
	}

	RenderedPolygon() {};

	void drawPolygon(HDC* context)
	{
		MoveToEx(*context, this->arr[0].x, this->arr[0].y, NULL);
		for (int i = 1; i < this->size; i++)
		{
			LineTo(*context, this->arr[i].x, this->arr[i].y);
		}
		LineTo(*context, this->arr[0].x, this->arr[0].y);
		delete[] arr;
	}

	void goToScreenCoord()
	{
		for (int i = 0; i < this->size; i++)
		{
			this->arr[i].x += ScreenWidth / 2;
			this->arr[i].y = ScreenHeigth / 2 - this->arr[i].y;
		}
	}

	/*void drawOnScreen(RenderedPolygon polygons[], int amount)
	{
		for (int i = 0; i < amount; i++)
		{
			drawSinglePolygon(polygons + i);
		}
	}*/

	~RenderedPolygon()
	{
		//delete[] res;
	}
};

class RenderableObject
{
private:
	_Polygon* parts;
	int size;

	friend class Transform;
public:
	RenderableObject(_Polygon arr[], int length)
	{
		this->size = length;

		parts = arr;
	}

	RenderedPolygon* render(ViewPoint* camera, float cam_matrix[4][4])
	{

		RenderedPolygon* polygons = new RenderedPolygon[size];

		for (int i = 0; i < size; i++)
		{
			polygons[i] = RenderedPolygon(parts[i].render(camera, cam_matrix), this->parts[i].size);
		}

		return polygons;
	}

	int getSize()
	{
		return size;
	}

	inline float findMax(AxisDirection axis)
	{
		float max = -DBL_MAX;
		switch (axis)
		{
		case X:
			for (int i = 0; i < getSize(); i++)
			{
				auto current_polygon = parts[i];
				for (int j = 0; j < current_polygon.size; j++)
				{
					Point3D p = current_polygon.points[j];
					if (p.x > max) {
						max = p.x;
					}
				}
			}
			break;
		case Y:
			for (int i = 0; i < getSize(); i++)
			{
				auto current_polygon = parts[i];
				for (int j = 0; j < current_polygon.size; j++)
				{
					Point3D p = current_polygon.points[j];
					if (p.y > max) {
						max = p.y;
					}
				}
			}
			break;
		case Z:
		{
			for (int i = 0; i < getSize(); i++)
			{
				auto current_polygon = parts[i];
				for (int j = 0; j < current_polygon.size; j++)
				{
					Point3D p = current_polygon.points[j];
					if (p.z > max) {
						max = p.z;
					}
				}
			}
			break;
		}
		}
		return max;
	}

	inline float findMin(AxisDirection axis)
	{
		float min = DBL_MAX;
		
		switch (axis)
		{
		case X:
			for (int i = 0; i < getSize(); i++)
			{
				auto current_polygon = parts[i];
				for (int j = 0; j < current_polygon.size; j++)
				{
					Point3D p = current_polygon.points[j];
					if (p.x < min) {
						min = p.x;
					}
				}
			}
			break;
		case Y:
			for (int i = 0; i < getSize(); i++)
			{
				auto current_polygon = parts[i];
				for (int j = 0; j < current_polygon.size; j++)
				{
					Point3D p = current_polygon.points[j];
					if (p.y < min) {
						min = p.y;
					}
				}
			}
			break;
		case Z:
		{
			for (int i = 0; i < getSize(); i++)
			{
				auto current_polygon = parts[i];
				for (int j = 0; j < current_polygon.size; j++)
				{
					Point3D p = current_polygon.points[j];
					if (p.z < min) {
						min = p.z;
					}
				}
			}
			break;
		}
		}
		return min;
	}


	~RenderableObject()
	{
		delete[] parts;
	}
};

class Axis
{
public:
	_Polygon line[3];
	Axis()
	{
		Point3D O(0, 0, 0);
		Point3D X(100, 0, 0);
		Point3D Y(0, 100, 0);
		Point3D Z(0, 0, 100);

		Point3D* arr1 = new Point3D[2]{ O,X };
		Point3D* arr2 = new Point3D[2]{ O,Y };
		Point3D* arr3 = new Point3D[2]{ O,Z };

		line[0] = _Polygon(arr1, 2);
		line[1] = _Polygon(arr2, 2);
		line[2] = _Polygon(arr3, 2);
	}
};

class Transform
{
private:
	static void _acceptMatrix(float matrix[4][4], RenderableObject* object);
public:
	static void moveOn(Point3D, RenderableObject*);
	static void rotate(float, AxisDirection, RenderableObject*);

};

void showPoint(Point2D point)
{
	cout << point.x << "  " << point.y << endl;
}

void update_view_matrix(float m[4][4], float omega, float fi_vert, float R)
{
	m[0][0] = -sin(omega);
	m[0][1] = -cos(fi_vert) * cos(omega);
	m[0][2] = -sin(fi_vert) * cos(omega);
	m[0][3] = 0;
	m[1][0] = cos(omega);
	m[1][1] = -cos(fi_vert) * sin(omega);
	m[1][2] = -sin(fi_vert) * sin(omega);
	m[1][3] = 0;
	m[2][0] = 0;
	m[2][1] = sin(fi_vert);
	m[2][2] = -cos(fi_vert);
	m[2][3] = 0;
	m[3][0] = 0;
	m[3][1] = 0;
	m[3][2] = R;
	m[3][3] = 1;
}

const int buff_size = 1024;

enum TypeOfObjInfo { Vrtx, Plgn, Undef};

bool isVertex(const string* str)
{
	return str->at(0) == 'v' && (str->at(1) != 'n' && str->at(1) != 't') ? true : false;
}

bool isOrderBypass(const string* str)
{
	return str->at(0) == 'f' ? true : false;
}

TypeOfObjInfo hasType(string* str)
{
	if (str->at(0) == 'v' && (str->at(1) != 'n' && str->at(1) != 't')) return TypeOfObjInfo::Vrtx;
	if (str->at(0) == 'f') return TypeOfObjInfo::Plgn;

	return TypeOfObjInfo::Undef;
}

Point3D convertVertex(string* str)
{
	float x = 0.0, y = 0.0, z = 0.0;

	int pos[4];
	pos[0] = str->find("  ");

	if (pos[0] == -1)
	{
		pos[0] = str->find(" ");
		pos[0] += 1;
	}
	//pos[0] += 2;

	pos[1] = str->find(" ", pos[0]);
	pos[2] = str->find(" ", pos[1] + 2);
	pos[3] = str->length();

	string x_str = str->substr(pos[0], pos[1] - pos[0]);
	string y_str = str->substr(pos[1] + 1, pos[2] - pos[1]);
	string z_str = str->substr(pos[2] + 1, pos[3] - pos[2]);

	x = stof(x_str);
	y = stof(y_str);
	z = stof(z_str);
	//cout << x << ' ' << y << ' ' << z << endl;

	return Point3D(x, y, z);
}

vector<int> convertPolygon(string& data)
{
	
	int start_pos = data.find(" ") + 1;

	string substr[500];

	int current_pos = 0;
	int counter = 0;


	// Create iterator pointing to first element
	while (current_pos != -1)
	{
		current_pos = data.find("/", start_pos);
		if (current_pos == -1) break;
		substr[counter++] = data.substr(start_pos, current_pos - start_pos);

		current_pos = data.find(" ", start_pos);

		start_pos = current_pos + 1;
	}

	vector<int> index(counter);

	for (int i = 0; i < counter; i++)
	{
		index[i] = stoi(substr[i]) - 1;
	}

	return index;
}

void firstRound(ifstream& modelFile, list<Point3D>& point_list)
{
	char buff[buff_size];
	while (!modelFile.eof())
	{
		modelFile.getline(buff, buff_size);

		string current_str(buff);

		if (!current_str.empty())
		{
			if (isVertex(const_cast<const string*>(&current_str)))
			{
				point_list.push_back(convertVertex(&current_str));
			}
		}
	}
	return;

}

template<class T>
T* listToArray(list<T>& list)
{
	T* arr = new T[list.size()];;
	int j = 0;
	for (T t : list)
	{
		arr[j++] = t;
	}

	return arr;
}

RenderableObject load_model(string filename)
{
	list<Point3D> pointsList;
	list<vector<int>> polygonsList;
	ifstream modelFile(filename);
	char buff[buff_size];

	while (!modelFile.eof())
	{
		modelFile.getline(buff, buff_size);

		string temp(buff);
		if (!temp.empty())
		{
			switch (hasType(&temp))
			{
			case Vrtx:
				pointsList.push_back(convertVertex(&temp));
				break;
			case Plgn:
				polygonsList.push_back(convertPolygon(temp));
				break;
			}
		}
	}

	Point3D* pointArr = listToArray<Point3D>(pointsList);
	pointsList.clear();
	int vertex_amount = pointsList.size();
	int polygon_amount = polygonsList.size();
	
	_Polygon* part = new _Polygon[polygonsList.size()];
	int counter = 0;

	for (auto current_plg : polygonsList)
	{
		int s = current_plg.size();
		Point3D* arr = new Point3D[s];
		for (int i = 0; i < s; i++)
		{
			arr[i] = pointArr[current_plg[i]];
		}
		part[counter++] = _Polygon(arr, s);
	}
	return RenderableObject(part, counter - 1);
}

void Transform::_acceptMatrix(float matrix[4][4], RenderableObject* o)
{
	for (int i = 0; i < o->getSize(); i++)
	{
		_Polygon* current_polygon = &o->parts[i];
		for (int j = 0; j < current_polygon->size; j++)
		{
			Point3D* current_point = &current_polygon->points[j];
			float point_vector[1][4] = { {current_point->x, current_point->y, current_point->z, 1} };
			float** res = Product::multiply_matrix(point_vector, 1, 4, matrix, 4, 4);

			current_point->x = res[0][0];
			current_point->y = res[0][1];
			current_point->z = res[0][2];

			Allocator::delete_arr((void**)res, 1);
		}
	}
}

void inline Transform::moveOn(Point3D delta, RenderableObject* object)
{
	/*float matrix[4][4] = {
		{1,0,0,0},
		{0,1,0,0},
		{0,0,1,0},
		{delta.x,delta.y,delta.z,1}
	};

	_acceptMatrix(matrix, object);*/
	for (int i = 0; i < object->getSize(); i++)
	{
		_Polygon* current_polygon = &object->parts[i];
		for (int j = 0; j < current_polygon->size; j++)
		{
			Point3D* current_point = &current_polygon->points[j];
	
			current_point->x += delta.x;
			current_point->y += delta.y;
			current_point->z += delta.z;
		}
	}
}

void Transform::rotate(float angle, AxisDirection direction, RenderableObject* object)
{
	float m[4][4];
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++)
		{
			m[i][j] = i == j ? 1 : 0;
		}
	}

	switch (direction)
	{
	case AxisDirection::X:
		m[1][1] = cos(angle);
		m[1][2] = sin(angle);
		m[2][1] = -sin(angle);
		m[2][2] = cos(angle);		
		break;
	case AxisDirection::Y:
		m[0][0] = cos(angle);
		m[0][2] = -sin(angle);
		m[2][0] = sin(angle);
		m[2][2] = cos(angle);
		break;
	case AxisDirection::Z:
		m[0][0] = cos(angle);
		m[0][1] = sin(angle);
		m[1][0] = -sin(angle);
		m[1][1] = cos(angle);
		break;
	}

	_acceptMatrix(m, object);
}
