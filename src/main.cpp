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
void task_4(int width, int height, vector<float> &posBuf, string filename);
void task_6(int width, int height, vector<float> &posBuf, vector<float> &norBuf, string filename);
void task_7(int width, int height, vector<float> &posBuf, vector<float> &norBuf, string filename);
void task_8(int width, int height, vector<float> &posBuf, vector<float> &norBuf, string filename);


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
            task_4(width, height, posBuf, filename);
            break;
        case 5:
            //task_5(width, height, posbuf, filename);
            break;
        case 6:
            task_6(width, height, posBuf, norBuf, filename);
            break;
        case 7:
            // code block
            task_7(width, height, posBuf, norBuf, filename);
            break;
        case 8:
            // code block
            //task_8(width, height, posBuf, norBuf, filename);
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

float triangle_mapping(float model_pos, float mid_model_pos, float half_image_pos, float ratio){
    float new_pos;
    // new_pos = model_pos * ratio + translation
    new_pos = model_pos * ratio + (half_image_pos - mid_model_pos);
    cout << "mapping" << " " << model_pos << " "<< new_pos << " " << half_image_pos - mid_model_pos << endl;
     return new_pos;
}

// edge function
float edgeFunction(Vertice a, Vertice b, Vertice c){
    return (c.x - a.x) * (b.y - a.y) - (c.y - a.y) * (b.x - a.x);
}

float alpha;
float beta;
float gamma;

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
}

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
        }
        for(float y = m_y_min; y <= m_y_max; y++){
            for(float x = m_x_min; x <= m_x_max; x++){
                Vertice p;
                p.x = x + 0.5f;
                p.y = y + 0.5f;
                p.z = 0;

                // edge function
                float w0 = edgeFunction(vertice[1], vertice[2], p);
                float w1 = edgeFunction(vertice[2], vertice[0], p);
                float w2 = edgeFunction(vertice[0], vertice[1], p);
                // check 7
                cout << w0 << " " << w1 << " " << w2 << endl;
                if (isInside(x, y, vertice[0], vertice[1], vertice[2])) {
                    //w0 /= area;
                    //w1 /= area;
                    //w2 /= area;
                    
                    w0 = alpha;
                    w1 = beta;
                    w2 = gamma;
                    int order = i%7;
                    unsigned char r = static_cast<unsigned char>(255 * RANDOM_COLORS[order][0]);
                    unsigned char g = static_cast<unsigned char>(255 * RANDOM_COLORS[order][1]);
                    unsigned char b = static_cast<unsigned char>(255 * RANDOM_COLORS[order][2]);
                    image->setPixel(x, y, r, g, b);
                    cout << "x: " << x << " y: " << y << endl;
                }
            }
        }
    }
    image->writeToFile(filename);
    return ;
}
