#include <iostream>
#include <string>

#define TINYOBJLOADER_IMPLEMENTATION
#include "tiny_obj_loader.h"

#include "Image.h"

#include <cstdlib>
#include <cmath>
#include <cfloat>

// This allows you to skip the `std::` in front of C++ standard library
// functions. You can also say `using std::cout` to be more selective.
// You should never do this in a header file.
using namespace std;

double RANDOM_COLORS[7][3] = {
	{0.0000,    0.4470,    0.7410},
	{0.8500,    0.3250,    0.0980},
	{0.9290,    0.6940,    0.1250},
	{0.4940,    0.1840,    0.5560},
	{0.4660,    0.6740,    0.1880},
	{0.3010,    0.7450,    0.9330},
	{0.6350,    0.0780,    0.1840},
};

class Vertice{
	public:	
		float x;
		float y;
		float z;

		Vertice(){}
		Vertice(float _x, float _y, float _z){
			x = _x;
			y = _y;
			z = _z;
		}
};

class Triangle{
	public:
		Vertice v1;
		Vertice v2;
		Vertice v3;
};


class Vec2{
	public:	
		float x;
		float y;
	
		Vec2(){}
		Vec2(float _x, float _y){
			x = _x;
			y = _y;
		}
};

class Vec3{
	public:	
		float x;
		float y;
		float z;

		Vec3(){}
		Vec3(float _x, float _y, float _z){
			x = _x;
			y = _y;
			z = _z;
		}
};

class Mat3x3{
	public:
		float m[3][3];
};

int mapping(float model_pos, float mid_model_pos, float half_image_pos, float ratio);
void float_compare(float a, float b);
void find_bounding(vector<float> &posBuf);
void model_bounding(vector<float> &posBuf, int idx);

// task function
void task_1(int width, int height, vector<float> &posBuf, string filename);
void task_2(int width, int height, vector<float> &posBuf, string filename);
void task_3(int width, int height, vector<float> &posBuf, string filename);


// task 2
void drawLine(int curx1, int curx2, int scanlineY, std::shared_ptr<Image> image, int order, float ratio);
void fillBottomFlatTriangle(Vertice v1, Vertice v2, Vertice v3, std::shared_ptr<Image> image, int order, float ratio);
void fillTopFlatTriangle(Vertice v1, Vertice v2, Vertice v3, std::shared_ptr<Image> image, int order, float ratio);

// task 3
float edgeFunction(Vertice a, Vertice b, Vertice c);
bool isInside(float Px, float Py, Vertice v1, Vertice v2, Vertice v3);
// task 4
//Vec3 inter_color();

// task 6
//void normal_color();

float x_min = FLT_MAX;
float x_max = FLT_MIN;
float y_min = FLT_MAX;
float y_max = FLT_MIN;

float m_x_min = FLT_MAX;
float m_x_max = FLT_MIN;
float m_y_min = FLT_MAX;
float m_y_max = FLT_MIN;

float half_width;    
float half_height;

float half_model_x;                 
float half_model_y;

int main(int argc, char **argv)
{
	if(argc < 2) {
		cout << "Usage: A1 meshfile" << endl;
		return 0;
	}
	string meshName(argv[1]);

	// store the parameters 
	// Output filename
	string filename(argv[2]);
	// Width of image
	int width = atoi(argv[3]);
	// Height of image
	int height = atoi(argv[4]);
	// Task number 
	int task_number = atoi(argv[5]);

	

	// vertex number 
	int vertex_number;

	// Load geometry
	vector<float> posBuf; // list of vertex positions
	vector<float> norBuf; // list of vertex normals
	vector<float> texBuf; // list of vertex texture coords
	tinyobj::attrib_t attrib;
	std::vector<tinyobj::shape_t> shapes;
	std::vector<tinyobj::material_t> materials;
	string errStr;
	bool rc = tinyobj::LoadObj(&attrib, &shapes, &materials, &errStr, meshName.c_str());
	if(!rc) {
		cerr << errStr << endl;
	} else {
		// Some OBJ files have different indices for vertex positions, normals,
		// and texture coordinates. For example, a cube corner vertex may have
		// three different normals. Here, we are going to duplicate all such
		// vertices.
		// Loop over shapes
		for(size_t s = 0; s < shapes.size(); s++) {
			// Loop over faces (polygons)
			size_t index_offset = 0;
			for(size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++) {
				size_t fv = shapes[s].mesh.num_face_vertices[f];
				// Loop over vertices in the face.
				for(size_t v = 0; v < fv; v++) {
					// access to vertex
					tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+0]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+1]);
					posBuf.push_back(attrib.vertices[3*idx.vertex_index+2]);
					if(!attrib.normals.empty()) {
						norBuf.push_back(attrib.normals[3*idx.normal_index+0]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+1]);
						norBuf.push_back(attrib.normals[3*idx.normal_index+2]);
					}
					if(!attrib.texcoords.empty()) {
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+0]);
						texBuf.push_back(attrib.texcoords[2*idx.texcoord_index+1]);
					}
				}
				index_offset += fv;
				// per-face material (IGNORE)
				shapes[s].mesh.material_ids[f];
			}
		}
	}
	cout << "Number of vertices: " << posBuf.size()/3 << endl;
   	vertex_number = posBuf.size()/3;
	
	switch(task_number) {
  		case 1:
			task_1(width, height, posBuf, filename);
    		break;
  		case 2:
			task_2(width, height, posBuf, filename);
    		break;
		case 3:
			task_3(width, height, posBuf, filename);
    		break;
		case 4:
			//task_4(width, height, posBuf, filename);
    		break;
		case 5:
			//task_4(width, height, posbuf, filename);
    		break;
		case 6:
    		// code block
			for(int i=0; i< vertex_number; i++){
				//task_6();
			}
    		break;
		case 7:
    		// code block
			for(int i=0; i< vertex_number; i++){
				//task_7();
			}
    		break;
		case 8:
			// code block
			for(int i=0; i< vertex_number; i++){
				//task_8();
			}
			break;
  		default:
			break;
   		// code block
	}
	return 0;
}


// compare two float values 
bool is_bigger(float a, float b){
		if(a>b && fabs(a-b)>1e-6){
			return true;
		}
		if(a<b && fabs(a-b)>1e-6){
			return false;
		}
	return false;
}

void model_bounding(vector<float> &posBuf, int idx){
	m_x_min = FLT_MAX;
	m_x_max = FLT_MIN;
	m_y_min = FLT_MAX;
	m_y_max = FLT_MIN; 
	for(int i=0; i<3; i++){
		int current_idx = idx*9+i*3;
		m_x_min = (is_bigger(m_x_min ,posBuf[current_idx]) == true) ? posBuf[current_idx] : m_x_min;
		m_x_max = (is_bigger(m_x_max ,posBuf[current_idx]) == true) ? m_x_max : posBuf[current_idx];
		m_y_min = (is_bigger(m_y_min ,posBuf[current_idx+1]) == true) ? posBuf[current_idx+1] : m_y_min;
		m_y_max = (is_bigger(m_y_max ,posBuf[current_idx+1]) == true) ? m_y_max : posBuf[current_idx+1];
		cout << posBuf[current_idx] << " " << posBuf[current_idx+1] << " " << posBuf[current_idx+2] << endl;
	}
	cout << "limit " << m_x_min << " " << m_x_max << endl;
	return ;
}

void find_bounding(vector<float> &posBuf){
	int size = posBuf.size()/3;
	for(int i=0; i<size; i++){
		int current_idx = i*3;
		x_min = (is_bigger(x_min ,posBuf[current_idx]) == true) ? posBuf[current_idx] : x_min;
		x_max = (is_bigger(x_max ,posBuf[current_idx]) == true) ? x_max : posBuf[current_idx];
		y_min = (is_bigger(y_min ,posBuf[current_idx+1]) == true) ? posBuf[current_idx+1] : y_min;
		y_max = (is_bigger(y_max ,posBuf[current_idx+1]) == true) ? y_max : posBuf[current_idx+1];
		cout << x_min << " " << x_max << endl;
		cout << "point" << i << endl;
		cout << posBuf[current_idx] << " " << posBuf[current_idx+1] << " " << posBuf[current_idx+2] << endl;
	}
	return ;
}

int mapping(float model_pos, float mid_model_pos, float half_image_pos, float ratio){
	float new_pos;
	// new_pos = model_pos * ratio + translation 
	new_pos = model_pos * ratio + (half_image_pos - mid_model_pos);
	cout << "mapping" << " " << model_pos << " "<< new_pos << " " << half_image_pos - mid_model_pos << endl;
 	return static_cast<int>(new_pos);
}

/*
float edgeFunction(const Vec2 &a, const Vec2 &b, const Vec2 &c){ 
	return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]); 
}*/


void drawLine(int int_curx1, int int_curx2, int scanlineY, std::shared_ptr<Image> image, int order, float ratio){
	// convert the color 
	unsigned char r = static_cast<unsigned char>(255*RANDOM_COLORS[order%7][0]);
	unsigned char g = static_cast<unsigned char>(255*RANDOM_COLORS[order%7][1]);
	unsigned char b = static_cast<unsigned char>(255*RANDOM_COLORS[order%7][2]);

	for(int i=int_curx1; i<=int_curx2; i++){
		image->setPixel(i, scanlineY, r, g, b);
	}
	return ;
}

// calculate the are of the "Bottom Flat"
void fillTopFlatTriangle(Vertice v1, Vertice v2, Vertice v3, std::shared_ptr<Image> image, int order, float ratio)
{
  float invslope1 = (v2.x - v1.x) / (v2.y - v1.y);
  float invslope2 = (v3.x - v1.x) / (v3.y - v1.y);

  float curx1 = v1.x;
  float curx2 = v1.x;

  // for loop to draw line 
  int v1_y = mapping(v1.y, half_model_y*ratio, half_height, ratio);
  int v2_y = mapping(v2.y, half_model_y*ratio, half_height, ratio);

  for (int scanlineY = v1_y; scanlineY <= v2_y; scanlineY++)
  {
	int int_curx1 = mapping(curx1, half_model_x*ratio, half_width, ratio);
	int int_curx2 = mapping(curx2, half_model_x*ratio, half_width, ratio);
	//cout << "draw top" << int_curx1 << " " << int_curx2 << " " << scanlineY <<endl;
    drawLine(int_curx1, int_curx2, scanlineY, image, order, ratio);
    curx1 += invslope1;
    curx2 += invslope2;
  }
  return ;
}

// calculate the are of the "top Flat"
void fillBottomFlatTriangle(Vertice v1, Vertice v2, Vertice v3, std::shared_ptr<Image> image, int order, float ratio)
{
  float invslope1 = (v3.x - v1.x) / (v3.y - v1.y);
  float invslope2 = (v3.x - v2.x) / (v3.y - v2.y);

  float curx1 = v3.x;
  float curx2 = v3.x;

  cout << "v3.x " << v3.x << endl;
  // for loop to draw line
  //y_start = mapping(m_y_min, half_model_y*ratio, half_height, ratio);
  int v1_y = mapping(v1.y, half_model_y*ratio, half_height, ratio);
  int v3_y = mapping(v3.y, half_model_y*ratio, half_height, ratio);

  for (int scanlineY = v3_y; scanlineY > v1_y; scanlineY--)
  {
	int int_curx1 = mapping(curx1, half_model_x*ratio, half_width, ratio);
	int int_curx2 = mapping(curx2, half_model_x*ratio, half_width, ratio);
	cout << "curx" <<  curx1 << " " << curx2  << " "<< half_model_x << " "<< half_width <<endl;
	cout << "draw bottom" << int_curx1 << " " << int_curx2 << " " << scanlineY <<endl;
    drawLine(int_curx1, int_curx2, scanlineY, image, order, ratio);
    curx1 -= invslope1;
    curx2 -= invslope2;
  }
  return ;
}

// task 3 interploation
/*
Vec3 inter_color(){
	for (uint32_t j = 0; j < h; ++j) {
        for (uint32_t i = 0; i < w; ++i) {
            Vec2 p = {i + 0.5f, j + 0.5f};
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
                framebuffer[j * w + i][0] = (unsigned char)(r * 255);
                framebuffer[j * w + i][1] = (unsigned char)(g * 255);
                framebuffer[j * w + i][2] = (unsigned char)(b * 255);
            }
        }
    }
}

Vec3 vertical_Color(){
	Vec3 bottom = new ();
	Vec3 top = new();

}
*/

/*
bool isInside(float Px, float Py, Triangle* &triangle, float& alpha, float& beta, float& gamma) {
	bool ans = false;
	//Checks where needs to be draw
	float topAlpha = (triangle->vert2->yCor - triangle->vert3->yCor) * (Px - triangle->vert3->xCor);
	topAlpha += (triangle->vert3->xCor - triangle->vert2->xCor) * (Py - triangle->vert3->yCor);

	float topBeta = (triangle->vert3->yCor - triangle->vert1->yCor) * (Px - triangle->vert3->xCor);
	topBeta += (triangle->vert1->xCor - triangle->vert3->xCor) * (Py - triangle->vert3->yCor);

	float area = (triangle->vert2->yCor - triangle->vert3->yCor) * (triangle->vert1->xCor - triangle->vert3->xCor);
	area += (triangle->vert3->xCor - triangle->vert2->xCor) * (triangle->vert1->yCor - triangle->vert3->yCor);

	alpha = topAlpha / area;
	beta = topBeta / area;
	gamma = 1 - alpha - beta;

	if (gamma >= -0.04 && beta >= -0.04 && alpha >= -0.04) {
		return true;
	}
	else {
		return false;
	}
}*/

void new_model_bounding(vector<float> &posBuf, int idx){
	int current_idx = idx*9;
	Vertice p1( posBuf[current_idx], posBuf[current_idx+1], posBuf[current_idx+2]);
	Vertice p2( posBuf[current_idx+3], posBuf[current_idx+4], posBuf[current_idx+5]);
	Vertice p3( posBuf[current_idx+6], posBuf[current_idx+7], posBuf[current_idx+8]);
	m_x_min = std::min({p1.x, p2.x, p3.x});
    m_x_max = std::max({p1.x, p2.x, p3.x});
    m_y_min = std::min({p1.y, p2.y, p3.y});
    m_y_max = std::max({p1.y, p2.y, p3.y});
	return ;
}

// Task 1: Drawing Bounding Boxes
void task_1(int width, int height, vector<float> &posBuf, string filename){
	
	// Create the image. We're using a `shared_ptr`, a C++11 feature.
	auto image = make_shared<Image>(width, height);

	// find the model bounding 
	find_bounding(posBuf);
	cout << "bounding" << endl;
	cout << x_min << " " << x_max << " " << y_min << " " << y_max <<endl;

	// find the ratio
	float x_bounding = abs(x_max - x_min);
	float y_bounding = abs(y_max - y_min);
	float x_ratio = (static_cast<float>(width)) / x_bounding;
	float y_ratio = (static_cast<float>(height)) / y_bounding;
	// choose the smaller one
	float ratio = (is_bigger(x_ratio, y_ratio) == true) ? y_ratio : x_ratio;
	
	// set the rectangle area 
	half_width = 0.5 * static_cast<float>(width);     // for x
	half_height = 0.5 * static_cast<float>(height);   // for y
	half_model_x = (x_min + x_max)/2;                 
	half_model_y = (y_min + y_max)/2;

	for(int i=0; i<(posBuf.size()/3)/3; i++){
		// draw the image 
		new_model_bounding(posBuf, i);
		cout << "halfmodel" << half_model_y << " " << half_model_x << endl;
		int y_start = mapping(m_y_min, half_model_y*ratio, half_height, ratio);
		int x_start = mapping(m_x_min, half_model_x*ratio, half_width, ratio);
		int y_end = mapping(m_y_max, half_model_y*ratio, half_height, ratio);
		int x_end = mapping(m_x_max, half_model_x*ratio, half_width, ratio);
		
		for(int y = y_start; y <= y_end; y++){
			for(int x = x_start; x <= x_end; x++){
				unsigned char r = static_cast<unsigned char>(255*RANDOM_COLORS[i%7][0]);
				unsigned char g = static_cast<unsigned char>(255*RANDOM_COLORS[i%7][1]);
				unsigned char b = static_cast<unsigned char>(255*RANDOM_COLORS[i%7][2]);
				image->setPixel(x, y, r, g, b);
			}
		}
	}
	image->writeToFile(filename);
	return ;
}

// Task 2: Drawing Triangles
// source: https://www.scratchapixel.com/lessons/3d-basic-rendering/rasterization-practical-implementation/rasterization-stage.html
void task_2(int width, int height, vector<float> &posBuf, string filename){
	auto image = make_shared<Image>(width, height);
	
	// find the bound
	find_bounding(posBuf);

	// find the ratio
	float x_bounding = abs(x_max - x_min);
	float y_bounding = abs(y_max - y_min);
	float x_ratio = static_cast<float>(width) / x_bounding;
	float y_ratio = static_cast<float>(height) / y_bounding;
	// choose the smaller one and get the ratio 
	float ratio = (is_bigger(x_ratio, y_ratio) == true) ? y_ratio : x_ratio;
	
	// translation
	half_width = 0.5 * static_cast<float>(width);     // for x
	half_height = 0.5 * static_cast<float>(height);   // for y
	half_model_x = (x_min + x_max)/2;                 
	half_model_y = (y_min + y_max)/2;

	for(int i=0; i<(posBuf.size()/3)/3; i++){
		// find the three pos and mapping it 
		Vertice vertice[4];
		for(int j=0; j<3; j++){
			vertice[j].x = posBuf[i*9+j*3];
			vertice[j].y = posBuf[i*9+j*3+1];
		}

		/* at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice */
		// sort Vertices Ascending By Y
		for(int k=0; k<3; k++){
			for(int g=k+1; g<3; g++){
				Vertice tmp(0, 0, 0);
				if(is_bigger(vertice[k].y, vertice[g].y) == true){
					tmp.x = vertice[k].x;
					tmp.y = vertice[k].y;
					vertice[k].x = vertice[g].x;
					vertice[k].y = vertice[g].y;
					vertice[g].x = tmp.x;
					vertice[g].y = tmp.y;
				}
			}
		}

	  //Vertice v4 = new Vertice((int)(vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) * (vt3.x - vt1.x)), vt2.y);
		

		if(vertice[1].y == vertice[2].y){
			fillBottomFlatTriangle(vertice[0], vertice[1], vertice[2], image, i, ratio);
		}else if(vertice[0].y == vertice[2].y){
			fillTopFlatTriangle(vertice[0], vertice[1], vertice[2], image, i, ratio);
		}else{
			vertice[3].x = vertice[0].x +  ((vertice[1].y - vertice[0].y) / (vertice[2].y - vertice[0].y)) * (vertice[2].x - vertice[0].x);
			vertice[3].y = vertice[1].y;
			fillBottomFlatTriangle(vertice[0], vertice[1], vertice[3], image, i, ratio);
			fillTopFlatTriangle(vertice[1], vertice[3], vertice[2], image, i, ratio);
		}

		cout << "v1" << " " << vertice[0].x << " " << vertice[0].y << endl;
		cout << "v2" << " " << vertice[1].x << " " << vertice[1].y << endl;
		cout << "v3" << " " << vertice[2].x << " " << vertice[2].y << endl;
		cout << "v4" << " " << vertice[3].x << " " << vertice[3].y << endl;
	}
       
		
	cout<< "ratio " <<  ratio << endl;
	image->writeToFile(filename);
	return ;
}

float triangle_mapping(float model_pos, float mid_model_pos, float half_image_pos, float ratio){
	float new_pos;
	// new_pos = model_pos * ratio + translation 
	new_pos = model_pos * ratio + (half_image_pos - mid_model_pos);
	cout << "mapping" << " " << model_pos << " "<< new_pos << " " << half_image_pos - mid_model_pos << endl;
 	return new_pos;
}


/*
bool isInside(float Px, float Py, Vertice v1, Vertice v2, Vertice v3) {
	//Checks whether or not the given x and y coordinates shoudl be drawn.
	float topAlpha = (v2.y - v3.y) * (Px - v3.x);
	topAlpha += (v3.x - v2.x) * (Py - v3.y);

	float topBeta = (v3.y - v1.y) * (Px - v3.x);
	topBeta += (v1.x - v3.x) * (Py - v3.y);

	float area = (v2.y - v3.y) * (v1.x - v3.x);
	area += (v3.x - v2.x) * (v1.y - v3.y);


	alpha = topAlpha / area;
	beta = topBeta / area;
	gamma = 1 - alpha - beta;

	if (gamma >= -0.04 && beta >= -0.04 && alpha >= -0.04) {
		return true;
	}
	else {
		return false;
	}
}*/



void task_3(int width, int height, vector<float> &posBuf, string filename){
	auto image = make_shared<Image>(width, height);
	
	// find the bound
	find_bounding(posBuf);

	// find the ratio
	float x_bounding = abs(x_max - x_min);
	float y_bounding = abs(y_max - y_min);
	float x_ratio = static_cast<float>(width) / x_bounding;
	float y_ratio = static_cast<float>(height) / y_bounding;
	// choose the smaller one and get the ratio 
	float ratio = (is_bigger(x_ratio, y_ratio) == true) ? y_ratio : x_ratio;
	
	// translation
	half_width = 0.5 * static_cast<float>(width);     // for x
	half_height = 0.5 * static_cast<float>(height);   // for y
	half_model_x = (x_min + x_max)/2;                 
	half_model_y = (y_min + y_max)/2;

	// check1
	cout << "half_width: " << half_width << " half_height: " << half_height << endl; 
    cout << "half_model_x: " << half_model_x << " half_model_y: " << half_model_y << endl; 
	
	// traverse each triangle
	for(int i=0; i<(posBuf.size()/3)/3; i++){

		// find the bounding of the triagle 
		new_model_bounding(posBuf, i);
	
		// check2
		cout << "m_x_min: " << m_x_min << " m_x_max: " << m_x_max << endl; 
    	cout << "m_y_min: " << m_y_min << " m_y_max: " << m_y_max << endl; 
		// initial the vertice
		// new_pos = model_pos * ratio + (half_image_pos - mid_model_pos);
		Vertice vertice[3];
		for(int j=0; j<3; j++){
			vertice[j].x = posBuf[i*9+j*3] * ratio + (half_width - half_model_x * ratio);
			vertice[j].y = posBuf[i*9+j*3+1] * ratio + (half_height - half_model_y * ratio);
		}
		float area = edgeFunction(vertice[0], vertice[1], vertice[2]);
        
		// go through each pixel for triangle bunding 
		m_y_min = m_y_min * ratio + (half_height - half_model_y * ratio);
		m_y_max = m_y_max * ratio + (half_height - half_model_y * ratio);
		m_x_min = m_x_min * ratio + (half_width - half_model_x * ratio);
		m_x_max = m_x_max * ratio + (half_width - half_model_x * ratio);
		// check3
		cout << "m_x_min: " << m_x_min << " m_x_max: " << m_x_max << endl; 
    	cout << "m_y_min: " << m_y_min << " m_y_max: " << m_y_max << endl; 
		// check4
		cout << "area: " << area << endl;

		// check5
		for(int j=0; j<3; j++){
			cout << j << " x " << vertice[j].x << endl;
			cout << j << " y " << vertice[j].y << endl;
		}/*
		for(float y = m_y_min; y <= m_y_max; y++){
			for(float x = m_x_min; x <= m_x_max; x++){

				// edge function 
				//float w0 = edgeFunction(vertice[1], vertice[2], p);
            	//float w1 = edgeFunction(vertice[2], vertice[0], p);
            	//float w2 = edgeFunction(vertice[0], vertice[1], p);
				//w0 /= area;
                //w1 /= area;
				//w2 = 1 - w0 - w1;
				// check 7
				//cout << w0 << " " << w1 << " " << w2 <<endl;
            	if (isInside(x, y, vertice[0], vertice[1], vertice[2]) == true) {
					float w0 = alpha;
					float w1 = beta;
					float w2 = gamma;
					cout << w0 << " " << w1 << " " << w2 <<endl;
               		unsigned char r = static_cast<unsigned char>(255 * (w0 * RANDOM_COLORS[0][0] + w1 * RANDOM_COLORS[1][0] + w2 * RANDOM_COLORS[2][0]));
                	unsigned char g = static_cast<unsigned char>(255 * (w0 * RANDOM_COLORS[0][1] + w1 * RANDOM_COLORS[1][1] + w2 * RANDOM_COLORS[2][1]));
                	unsigned char b = static_cast<unsigned char>(255 * (w0 * RANDOM_COLORS[0][2] + w1 * RANDOM_COLORS[1][2] + w2 * RANDOM_COLORS[2][2]));
					image->setPixel(x, y, r, g, b);
					cout << "x: " << x << " y: " << y << endl;
                }
			}
		}*/
	}
	image->writeToFile(filename);
	return ;
}


/*
void task_3(int width, int height, vector<float> &posBuf, string filename){
	auto image = make_shared<Image>(width, height);
	
	// find the bound
	find_bounding(posBuf);

	// find the ratio
	float x_bounding = abs(x_max - x_min);
	float y_bounding = abs(y_max - y_min);
	float x_ratio = static_cast<float>(width) / x_bounding;
	float y_ratio = static_cast<float>(height) / y_bounding;
	// choose the smaller one and get the ratio 
	float ratio = (is_bigger(x_ratio, y_ratio) == true) ? y_ratio : x_ratio;
	// translation
	float half_width = 0.5 * static_cast<float>(width);     // for x
	float half_height = 0.5 * static_cast<float>(height);   // for y
	float half_model_x = (x_min + x_max)/2;                 
	float half_model_y = (y_min + y_max)/2;

	for(int i=0; i<(posBuf.size()/3)/3; i++){
		// find the three pos and mapping it 
		Vertice vertice[4];
		for(int j=0; j<3; j++){
			vertice[j].x = posBuf[i*3+j];
			vertice[j].y = posBuf[i*3+j];
		}

		// at first sort the three vertices by y-coordinate ascending so v1 is the topmost vertice 
		// sortVerticesAscendingByY();
		for(int k=0; k<3; k++){
			for(int g=k+1; g<3; g++){
				Vertice tmp;
				if(is_bigger(vertice[vertice[k].y], vertice[g].y) == true){
					tmp.x = vertice[k].x;
					tmp.y = vertice[k].y;
					vertice[k] = vertice[g];
					vetrtce[k] = vertice[g];
					vetrtce[g] = tmp.x;
					vetrtce[g] = tmp.y;
				}
			}
		}

		if (vertice[1].y == vertice[2].y)
		{
			fillBottomFlatTriangle(vertice[0], vertice[1], vertice[2], image, ratio);
		}
		// check for trivial case of top-flat triangle 
		else if (vertice[2].y == verticep[3].y)
		{
			fillTopFlatTriangle(vertice[0], vertice[1], vertice[2], image, ratio);
		}
		else
		{
			// general case - split the triangle in a topflat and bottom-flat one 
			vertice[3] = new Vertice((vertice[0].x + (vertice[1].y - vertice[0].y) / (vertice[2].y - vertice[0].y)) * (vertice[2].x - vertice[0].x)), vertice[1].y, 0);
			fillBottomFlatTriangle(vertice[0], vertice[1], vertice[3], image, ratio);
			fillTopFlatTriangle(vertice[1], vertice[3], vertice[2], image, ratio);
		}
	}
	return;
}
*/
/*

typedef float Vec2[2];
typedef float Vec3[3];
typedef unsigned char Rgb[3];

inline
float edgeFunction(const Vec2 &a, const Vec2 &b, const Vec2 &c)
{ return (c[0] - a[0]) * (b[1] - a[1]) - (c[1] - a[1]) * (b[0] - a[0]); }

int main(int argc, char **argv)
{
    Vec2 v0 = {491.407, 411.407};
    Vec2 v1 = {148.593, 68.5928};
    Vec2 v2 = {148.593, 411.407};
    Vec3 c0 = {1, 0, 0};
    Vec3 c1 = {0, 1, 0};
    Vec3 c2 = {0, 0, 1};
    
    const uint32_t w = 512;
    const uint32_t h = 512;
    
    Rgb *framebuffer = new Rgb[w * h];
    memset(framebuffer, 0x0, w * h * 3);
    
    float area = edgeFunction(v0, v1, v2);
    
    for (uint32_t j = 0; j < h; ++j) {
        for (uint32_t i = 0; i < w; ++i) {
            Vec2 p = {i + 0.5f, j + 0.5f};
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
                framebuffer[j * w + i][0] = (unsigned char)(r * 255);
                framebuffer[j * w + i][1] = (unsigned char)(g * 255);
                framebuffer[j * w + i][2] = (unsigned char)(b * 255);
            }
        }
    }
    
    std::ofstream ofs;
    ofs.open("./raster2d.ppm");
    ofs << "P6\n" << w << " " << h << "\n255\n";
    ofs.write((char*)framebuffer, w * h * 3);
    ofs.close();
    
    delete [] framebuffer;
    
    return 0; 
}

}


// Task 3: Interpolating Per-Vertex Colors
void task_3(int width, int height, vector<float> &posBuf, string filename){
	auto image = make_shared<Image>(width, height);
	
	// find the bound
	find_bounding(posBuf);

	// find the ratio
	float x_bounding = abs(x_max - x_min);
	float y_bounding = abs(y_max - y_min);
	float x_ratio = static_cast<float>(width) / x_bounding;
	float y_ratio = static_cast<float>(height) / y_bounding;
	// choose the smaller one
	float ratio = (is_bigger(x_ratio, y_ratio) == true) ? y_ratio : x_ratio;



	// draw the image 
	Vec2 v0 = {491.407, 411.407};
    Vec2 v1 = {148.593, 68.5928};
    Vec2 v2 = {148.593, 411.407};
    Vec3 c0 = {1, 0, 0};
    Vec3 c1 = {0, 1, 0};
    Vec3 c2 = {0, 0, 1};
    
    const uint32_t w = 512;
    const uint32_t h = 512;
    
    Rgb *framebuffer = new Rgb[w * h];
    memset(framebuffer, 0x0, w * h * 3);
    
    float area = edgeFunction(v0, v1, v2);
    
    for (uint32_t j = 0; j < h; ++j) {
        for (uint32_t i = 0; i < w; ++i) {
            Vec2 p = {i + 0.5f, j + 0.5f};
            float w0 = edgeFunction(v1, v2, p);
            float w1 = edgeFunction(v2, v0, p);
            float w2 = edgeFunction(v0, v1, p);
            if (w0 >= 0 && w1 >= 0 && w2 >= 0) {
                w0 /= area;
                w1 /= area;
                w2 /= area;
                float r = w0 * c0[0] + w1 * c1[0] + w2 * c2[0];
                float g = w0 * c0[1] + w1 * c1[1] + w2 * c2[1];
                float b = w0 * c0[2] + w1 * c1[2] + w2 * c2[2];
                framebuffer[j * w + i][0] = (unsigned char)(r * 255);
                framebuffer[j * w + i][1] = (unsigned char)(g * 255);
                framebuffer[j * w + i][2] = (unsigned char)(b * 255);
            }
        }
    }

	image->writeToFile(filename);
	return;
}




// Task 4: Vertical Color
void task_4(int width, int height, ){
	auto image = make_shared<Image>(width, height);
	
	// find the bound
	find_bounding(posBuf);

	// find the ratio
	float x_bounding = abs(x_max - x_min);
	float y_bounding = abs(y_max - y_min);
	float x_ratio = static_cast<float>(width) / x_bounding;
	float y_ratio = static_cast<float>(height) / y_bounding;
	// choose the smaller one
	float ratio = (is_bigger(x_ratio, y_ratio) == true) ? y_ratio : x_ratio;


	for(int i=0; i<(posBuf.size()/3)/3; i++){
		// draw the image 
		int y_start = mapping(y_min*ratio, half_model_y*ratio, half_height, ratio);
		int x_start = mapping(x_min*ratio, half_model_x*ratio, half_width, ratio);
		// find the triangle bouding
		int y_draw_bound = static_cast<int>(y_bounding * ratio);
		int x_draw_bound = static_cast<int>(x_bounding * ratio);
		cout << "start point" << y_start << " " << x_start << endl;
		cout << "draw bound" << y_draw_bound << " " << x_draw_bound << endl;
		cout << "y:" << y_start << "x:" << x_start <<endl;
		
		for(int y = y_start; y < y_draw_bound; y++){
			for(int x = x_start; x < x_draw_bound; x++){
				int tmp_r = 0* + 255* ;
				int tmp_g = 0;
				int tmp_b = 255* + 0*;
				unsigned char r = static_cast<unsigned char>(tmp_r);
				unsigned char g = static_cast<unsigned char>(tmp_g);
				unsigned char b = static_cast<unsigned char>(tmp_b);
				//cout << "color" << endl; 
				//cout << r << " " << g << " " << b << endl;
				image->setPixel(x, y, r, g, b);
			}
		}
	}

	image->writeToFile(filename);
	return;
}



float ZCheck(){
	retrun z;
}
// Task 5: Z-Buffering
void task_5(){

}

// Task 6: Normal Coloring
void task_6(){
	
	r = 255 * (0.5 * x + 0.5);
	g = 255 * (0.5 * y + 0.5);
	b = 255 * (0.5 * z + 0.5);

}

// Task 7: Simple Lighting
void task_7(int width, int height, ){
	// lighting 
	float l[3] = {1/sqrt(3), 1/sqrt(3), 1/sqrt(3)};
	float tmp_r = is_bigger(dotProduct(), 0);
	float tmp_g = is_bigger(dotProduct(), 0);
	float tmp_b = is_bigger(dotProduct(), 0);
	// draw the image 
	// static_cast<int>(floatValue)
	for(int y = y_min; y < y_bounding; y++){
		for(int x = x_min; x < x_bounding; x++){
			unsigned char r = static_cast<unsigned char>(RANDOM_COLORS[v_order][0]);
			unsigned char g = static_cast<unsigned char>(RANDOM_COLORS[v_order][1]);
			unsigned char b = static_cast<unsigned char>(RANDOM_COLORS[v_order][2]);
			image->setPixel(x, y, r, g, b);
		}
	}
}

// Task 8: Rotation
void task_8(){
	vec3 result = multiplyMatrixVector(matrix, vector);

	// Convert degrees to radians
	double angle = 45.0 * M_PI / 180.0; 
    // Calculate sine and cosine
    float p_sinValue = std::sin(angle);
    float p_cosValue = std::cos(angle);
	float n_sinValue = std::sin(angle);
	float n_cosValue = std::sin(angle);

    // Display results
    std::cout << "Sine of " << angle << " radians: " << sinValue << std::endl;
    std::cout << "Cosine of " << angle << " radians: " << cosValue << std::endl;
	return ;


}


// rotate multiple with vec3
struct vec3 {
    float x, y, z;
};

// Define a 3x3 matrix type
using Mat3x3 = std::vector<std::vector<float>>;

// Function to perform matrix-vector multiplication
vec3 multiplyMatrixVector(Mat3x3& matrix,  Vec3& vector) {
    vec3 result;

    result.x = matrix[0][0] * vector.x + matrix[0][1] * vector.y + matrix[0][2] * vector.z;
    result.y = matrix[1][0] * vector.x + matrix[1][1] * vector.y + matrix[1][2] * vector.z;
    result.z = matrix[2][0] * vector.x + matrix[2][1] * vector.y + matrix[2][2] * vector.z;

    return result;

}

int main() {
    // Example 3x3 matrix
    Mat3x3 matrix = {
        {1.0f, 2.0f, 3.0f},
        {4.0f, 5.0f, 6.0f},
        {7.0f, 8.0f, 9.0f}
    };

    // Example 3D vector
    vec3 vector = {2.0f, 3.0f, 4.0f};

    // Perform matrix-vector multiplication
    vec3 result = multiplyMatrixVector(matrix, vector);

    // Display the result
    std::cout << "Result: (" << result.x << ", " << result.y << ", " << result.z << ")" << std::endl;

    return 0;
}


float dotProduct(const std::vector<float>& a, const std::vector<float>& b) {
    // Check if the vectors have the same size
    if (a.size() != b.size()) {
        std::cerr << "Error: Vectors must have the same size for dot product." << std::endl;
        return 0.0f; // Return 0 as an error value
    }

    float result = 0.0f;

    // Calculate the dot product
    for (size_t i = 0; i < a.size(); ++i) {
        result += a[i] * b[i];
    }

    return result;
}

*/