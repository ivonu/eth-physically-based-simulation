#include <iostream>
#include "mesh.h"
#include <stdlib.h>
#include <fstream>
#include "utilities.h"

//
// Mesh Definitions
//
Mesh::Mesh()
: m_data(0), m_indices(0)
{
  loadOFF("mesh005.off");
}

// Parser
void Mesh::loadOFF(string filename)
{
// Container holding last line read
  string readLine;

  // Containers for delimiter positions
  int delimiterPos_1, delimiterPos_2, delimiterPos_3, delimiterPos_4;

  // Open file for reading
  ifstream in(filename.c_str());

  // Check if file is in OFF format
  getline(in,readLine);
  if (readLine != "OFF")
  {
    cout << "The file to read is not in OFF format." << endl;
    return;
  }

  // Read values for Nv and Nf
  getline(in,readLine);
  delimiterPos_1 = readLine.find(" ", 0);
  nv = atoi(readLine.substr(0,delimiterPos_1+1).c_str());

  delimiterPos_2 = readLine.find(" ", delimiterPos_1);
  nf = atoi(readLine.substr(delimiterPos_1,delimiterPos_2 +1).c_str());

  m_data = new GLfloat[ nv*3 ];  
  m_normals = new GLfloat[ nv*3 ];  
  m_indices = new GLuint[ nf*3 ];
  
  // Read the vertices
  for (int n=0; n<nv; n++)
  {
    getline(in,readLine);
    
    // x
    delimiterPos_1 = readLine.find(" ", 0);
    m_data[n*3] = atof(readLine.substr(0,delimiterPos_1).c_str());

    // y
    delimiterPos_2 = readLine.find(" ", delimiterPos_1+1);
    m_data[n*3 + 1] = atof(readLine.substr(delimiterPos_1,delimiterPos_2 ).c_str());

    // z
    delimiterPos_3 = readLine.find(" ", delimiterPos_2+1);
    m_data[n*3 + 2] = atof(readLine.substr(delimiterPos_2,delimiterPos_3 ).c_str()) - 11;

    m_normals[n*3] = 0;
    m_normals[n*3+1] = 0;
    m_normals[n*3+2] = 0;
  }

  // Read the faces
  for (int n=0; n<nf; n++)
  {
    getline(in,readLine);
    
    // v1
    delimiterPos_1 = readLine.find(" ", 0);
    delimiterPos_2 = readLine.find(" ", delimiterPos_1+1);
    int v1 = atoi(readLine.substr(delimiterPos_1,delimiterPos_2 ).c_str());
    m_indices[n*3] = v1;

    // v2
    delimiterPos_3 = readLine.find(" ", delimiterPos_2+1);
    int v2 = atoi(readLine.substr(delimiterPos_2,delimiterPos_3 ).c_str());
    m_indices[n*3 + 1] = v2;

    // v3
    delimiterPos_4 = readLine.find(" ", delimiterPos_3+1);
    int v3 = atoi(readLine.substr(delimiterPos_3,delimiterPos_4 ).c_str());
    m_indices[n*3 + 2] = v3;

    Vector3f e1 (m_data[v1*3] - m_data[v2*3], m_data[v1*3 + 1] - m_data[v2*3 + 1], m_data[v1*3 + 2] - m_data[v2*3 + 2]);
    Vector3f e2 (m_data[v3*3] - m_data[v2*3], m_data[v3*3 + 1] - m_data[v2*3 + 1], m_data[v3*3 + 2] - m_data[v2*3 + 2]);
    Vector3f normal = e1.cross(e2);

    m_normals[v1*3] += normal.x();
    m_normals[v1*3+1] += normal.y();
    m_normals[v1*3+2] += normal.z();
  }

  for (int n=0; n<nv; n++) {
    Vector3f normal(m_normals[n*3], m_normals[n*3+1], m_normals[n*3+2]);
    normal.normalize();

    m_normals[n*3]   = -normal.x();
    m_normals[n*3+1] = -normal.y();
    m_normals[n*3+2] = -normal.z();
  }
}

void Mesh::initialize( void )
{
// query the OpenGL driver about the maximum number of vertices
// recommended for a vertex array and the recommended maximum for
// number of indices.
  glGetIntegerv(GL_MAX_ELEMENTS_VERTICES, &m_max_vertices);
  glGetIntegerv(GL_MAX_ELEMENTS_INDICES, &m_max_indices);

// may need to tailor this code to take the above two values into
// account... creating multiple vertex arrays to easily fit into the
// drivers (and card's) caches.
  std::cout << "Recommended maximum number of vertex array vertices: " << m_max_vertices << std::endl;
  std::cout << "Recommended maximum number of vertex array indices: " << m_max_indices << std::endl;

// enable the client state for the vertex array and set the pointer
  glEnableClientState(GL_VERTEX_ARRAY);
  glEnableClientState(GL_NORMAL_ARRAY);
  glVertexPointer(3, GL_FLOAT, 0, m_data);
  glNormalPointer(GL_FLOAT, 0, m_normals);
}

void Mesh::render( void )
{
  // Render as LINES to see the mesh.  Comment out this line to render as full triangles.
  // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);  

  // glDrawElements renders primitives from array data with the first
  // arg being the primitive mode (such as points, tris, etc...), and
  // the second arg being the number of "elements" to be rendered.
  // Elements refers to the number of indices to utilize. So, for
  // triangles, this is essentially the number of triangles multiplied
  // by the number of vertices.  For tristrips, the number will be
  // less. The third and fourth arg referring to the type of values in
  // the indice array and the indice array, respectively.

  // Draw the triangles using indices into the data array...
  glColor3f(1.0, 0.0, 0.0);
  glDrawElements(GL_TRIANGLES, nf*3, GL_UNSIGNED_INT, m_indices);

}

Mesh::~Mesh()
{
  delete [] m_data;
  delete [] m_indices;
  delete [] m_normals;
}
