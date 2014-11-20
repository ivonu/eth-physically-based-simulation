#ifndef __PETESMESH_H__
#define __PETESMESH_H__ 1

#include "GLUT/glut.h"
#include <string>

using namespace std;

class Mesh
{
public:
  Mesh();
  ~Mesh();

  void initialize( void );
  void render( void );
  void loadOFF (string filename);

private:
  int nv;
  int nf;

  GLfloat *m_data;       // contains the data for the vertices of the mesh
  GLfloat *m_normals;   // contains the data for the normals of the vertices of the mesh
  GLuint *m_indices;    // contains the indices of the triangles in the m_data array
  
  GLint m_max_vertices;
  GLint m_max_indices;
};

#endif
