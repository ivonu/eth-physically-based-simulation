#include "scene.h"
#include "utilities.h"
#include "GLUT/glut.h"
#include <algorithm>
#include <cmath>

const Vector3d Scene::initial_pos (0, 0.7, -10.5);
const int Scene::NUM_PARTICLES_X = 3;
const int Scene::NUM_PARTICLES_Y = 2;
const int Scene::NUM_PARTICLES_Z = 3;
const int Scene::NUM_PARTICLES = NUM_PARTICLES_X * NUM_PARTICLES_Y * NUM_PARTICLES_Z;
const double Scene::BARRIER = -0.5;
const double Scene::LEFT_WALL  = -1.5;
const double Scene::RIGHT_WALL = 1.5;
const double Scene::BACK_WALL  = -11;
const double Scene::FRONT_WALL = -10;
const double Scene::BOTTOM_WALL = -1.5;
const double Scene::TOP_WALL = 1.5;
const double Scene::d = 0.1;
const double Scene::h = 2.0 * d;
const double Scene::volume = d*d*d;
const double Scene::rho0 = 1000.0;
const double Scene::k = 1000.0;
const double Scene::mu = 80;
const double Scene::collision_damping = 1.0;

const int Scene::timestep = 7;

Scene::Scene(void) :
	grid(Vector3d(RIGHT_WALL-LEFT_WALL, TOP_WALL-BOTTOM_WALL, FRONT_WALL-BACK_WALL), Vector3d(LEFT_WALL, BOTTOM_WALL, FRONT_WALL), h),
	objmodel_ptr(NULL)
{
   Init();
}

Scene::~Scene(void) {
	for (int i = 0; i < NUM_PARTICLES; i++) {
		delete particles[i];
	}
	
	for (int i = 0; i < collision_objects.size(); i++) {
		delete collision_objects[i];
	}
	
	for (int i = 0; i < collision_bounds.size(); i++) {
		delete collision_bounds[i];
	}
}

void Scene::Init(void)
{
   //Animation settings
   pause=false;

   for (int x = 0; x < NUM_PARTICLES_X; x++) {
	   for (int y = 0; y < NUM_PARTICLES_Y; y++) {
	   		for (int z = 0; z < NUM_PARTICLES_Z; z++) {
	   			Particle* part = new Particle(initial_pos + Vector3d(x*d*1, y*d*1, -z*d*1));
   		// 		if (y >= NUM_PARTICLES_Y/2) {
   		// 			part->rho0 = 1500;
					// part->mass = Scene::volume * part->rho0;
					// part->color = Vector3d(0,1,0);
   		// 		}
   				particles.push_back(part);
				grid.addParticle(part);
   			}
   		}
  	}

  	collision_bounds.push_back (new CollisionPlane (Vector3d(LEFT_WALL, BOTTOM_WALL, FRONT_WALL), Vector3d(0,1,0)));
  	collision_bounds.push_back (new CollisionPlane (Vector3d(LEFT_WALL, TOP_WALL, FRONT_WALL), Vector3d(0,-1,0)));
  	collision_bounds.push_back (new CollisionPlane (Vector3d(LEFT_WALL, BOTTOM_WALL, FRONT_WALL), Vector3d(1,0,0)));
  	collision_bounds.push_back (new CollisionPlane (Vector3d(RIGHT_WALL, BOTTOM_WALL, FRONT_WALL), Vector3d(-1,0,0)));
  	collision_bounds.push_back (new CollisionPlane (Vector3d(LEFT_WALL, BOTTOM_WALL, FRONT_WALL), Vector3d(0,0,-1)));
  	collision_bounds.push_back (new CollisionPlane (Vector3d(LEFT_WALL, BOTTOM_WALL, BACK_WALL), Vector3d(0,0,1)));

  	// load mesh
  	if (!objmodel_ptr) {
	    objmodel_ptr = glmReadOBJ((char*)"bunny.obj", 0,-1,-11);
	    if (!objmodel_ptr)
	        exit(0);

	    glmUnitize(objmodel_ptr);
	    glmFacetNormals(objmodel_ptr);
	    glmVertexNormals(objmodel_ptr, 90.0);

	    for (int i = 0; i < objmodel_ptr->numtriangles; i++) {
	    	GLMtriangle triangle = objmodel_ptr->triangles[i];

	    	CollisionTriangle* col_triangle = new CollisionTriangle(
	    		Vector3d(objmodel_ptr->vertices[triangle.vindices[0]*3],
			    		 objmodel_ptr->vertices[triangle.vindices[0]*3+1], 
			    		 objmodel_ptr->vertices[triangle.vindices[0]*3+2]), 

	    		Vector3d(objmodel_ptr->vertices[triangle.vindices[1]*3],
			    		 objmodel_ptr->vertices[triangle.vindices[1]*3+1], 
			    		 objmodel_ptr->vertices[triangle.vindices[1]*3+2]), 

	    		Vector3d(objmodel_ptr->vertices[triangle.vindices[2]*3],
			    		 objmodel_ptr->vertices[triangle.vindices[2]*3+1], 
			    		 objmodel_ptr->vertices[triangle.vindices[2]*3+2]));

	    	// cout << "v1: " << col_triangle->v1.x() << " / " << col_triangle->v1.y() << " / " << col_triangle->v1.z() << endl;
	    	// cout << "v2: " << col_triangle->v2.x() << " / " << col_triangle->v2.y() << " / " << col_triangle->v2.z() << endl;
	    	// cout << "v3: " << col_triangle->v3.x() << " / " << col_triangle->v3.y() << " / " << col_triangle->v3.z() << endl;

			collision_objects.push_back(col_triangle);
	    }
	}
}

double Scene::poly6_kernel(double r) {
	if (r < 0 || r > h) 
		return 0;

	return (315.0 / (64.0 * M_PI * pow (h, 9))) * pow(h*h - r*r, 3);
}

double Scene::spiky_kernel_gradient(double r) {
	if (r < 0 || r > h) 
		return 0.0;

	return -3.0 * (15.0 / (M_PI * pow(h, 6))) * pow(h-r, 2);
}

double Scene::viscosity_laplacian(double r) {
	if (r < 0 || r > h) 
		return 0;

	return (45.0 / (M_PI * pow(h, 6))) * (h-r);
}


vector<Particle*> Scene::findNeighboors (const vector<Particle*>& potential_neighboors, Particle* particle) {
	vector<Particle*> neighboors;
	for (int i = 0; i < potential_neighboors.size(); i++) {
		if ((particle->position - potential_neighboors[i]->position).length() <= h) {
			neighboors.push_back(potential_neighboors[i]);
		}
	}
	return neighboors;
}


void Scene::Update(void)
{
	if (pause) {
	  return;
	}
	vector< vector<Particle*> > neighboors (particles.size());
	for (int i = 0; i < NUM_PARTICLES; i++) {
		// find neighborhoods Ni(t)
		//neighboors[i] = findNeighboors(grid.getNeighboors(particles[i]), particles[i]);
		neighboors[i] = findNeighboors(particles, particles[i]);
		
		// compute density
		particles[i]->density = 0.0;
		for (int j = 0; j < neighboors[i].size(); j++) {
			particles[i]->density += neighboors[i][j]->mass * poly6_kernel((particles[i]->position - neighboors[i][j]->position).length());
		}
		
		// compute pressure
		particles[i]->pressure = k * (particles[i]->density - particles[i]->rho0);
		particles[i]->pressure = max(particles[i]->pressure, 0.0);
	}

	// compute forces
	for (int i = 0; i < NUM_PARTICLES; i++) {

		Vector3d pressure_force (0.0,0.0,0.0);
		Vector3d viscosity_force (0.0,0.0,0.0);
		for (int j = 0; j < neighboors[i].size(); j++) {
			Vector3d x_ij = (particles[i]->position - neighboors[i][j]->position);

			double pressure_force_scalar = (neighboors[i][j]->mass / neighboors[i][j]->density) *
				((particles[i]->pressure + neighboors[i][j]->pressure) / 2.0) * 
				spiky_kernel_gradient(x_ij.length());

			if (x_ij.length() != 0.0)
				pressure_force -= x_ij.normalized() * pressure_force_scalar;
			
			viscosity_force += mu * 
				(neighboors[i][j]->mass / neighboors[i][j]->density) * 
				(neighboors[i][j]->speed - particles[i]->speed) * 
				viscosity_laplacian(x_ij.length());
		}
		
		Vector3d gravitation_force = particles[i]->density * Vector3d(0,-9.81,0);

		particles[i]->force = Vector3d(0,0,0);
		particles[i]->force += pressure_force;
		particles[i]->force += viscosity_force;
		particles[i]->force += gravitation_force;

		if (particles[i]->force.length() <= 0 || particles[i]->density <= 0) {
			cout << "force: " << particles[i]->force.x() << " / " << particles[i]->force.y() << " / " << particles[i]->force.z() << endl;
			cout << "pres-force: " << pressure_force.x() << " / " << pressure_force.y() << " / " << pressure_force.z() << endl;
			cout << "visc-force: " << viscosity_force.x() << " / " << viscosity_force.y() << " / " << viscosity_force.z() << endl;
			cout << "grav-force: " << gravitation_force.x() << " / " << gravitation_force.y() << " / " << gravitation_force.z() << endl;
			cout << "dens: " << particles[i]->density << endl;
		}
	}

	grid.removeParticles();

	double dt = timestep / 1000.0;
	for (int i = 0; i < NUM_PARTICLES; i++) {
		// update velocities and positions

		cout << "old pos: " << particles[i]->position.x() << " / " << particles[i]->position.y() << " / " << particles[i]->position.z() << endl;
		cout << "old speed: " << particles[i]->speed.x() << " / " << particles[i]->speed.y() << " / " << particles[i]->speed.z() << endl;
		cout << "force: " << particles[i]->force.x() << " / " << particles[i]->force.y() << " / " << particles[i]->force.z() << endl;
		cout << "dens: " << particles[i]->density << endl;
		particles[i]->speed += particles[i]->force * (dt / particles[i]->density);
		particles[i]->position += (particles[i]->speed * dt);
		cout << "new pos: " << particles[i]->position.x() << " / " << particles[i]->position.y() << " / " << particles[i]->position.z() << endl;
		cout << "new speed: " << particles[i]->speed.x() << " / " << particles[i]->speed.y() << " / " << particles[i]->speed.z() << endl;

		for (int c = 0; c < collision_objects.size(); c++) {
			if (collision_objects[c]->handleCollision(particles[i], dt)) 
				break;
		}

		for (int c = 0; c < collision_bounds.size(); c++) {
			collision_bounds[c]->handleCollision(particles[i], dt);
		}

		grid.addParticle(particles[i]);
	}
}

void Scene::Render(void)
{

	// // BOTTOM
	// glColor3d(1,1,1);
	// glBegin(GL_QUADS);
	// 	glVertex3f(LEFT_WALL, BOTTOM_WALL, FRONT_WALL);
	// 	glVertex3f(RIGHT_WALL, BOTTOM_WALL, FRONT_WALL);
	// 	glVertex3f(RIGHT_WALL, BOTTOM_WALL, BACK_WALL);
	// 	glVertex3f(LEFT_WALL, BOTTOM_WALL, BACK_WALL);
	// glEnd();
	// // LEFT
	// glColor3d(1,0,1);
	// glBegin(GL_QUADS);
	// 	glVertex3f(LEFT_WALL, BOTTOM_WALL, FRONT_WALL);
	// 	glVertex3f(LEFT_WALL, BOTTOM_WALL, BACK_WALL);
	// 	glVertex3f(LEFT_WALL, TOP_WALL, BACK_WALL);
	// 	glVertex3f(LEFT_WALL, TOP_WALL, FRONT_WALL);
	// glEnd();
	// // RIGHT
	// glColor3d(1,1,0);
	// glBegin(GL_QUADS);
	// 	glVertex3f(RIGHT_WALL, BOTTOM_WALL, FRONT_WALL);
	// 	glVertex3f(RIGHT_WALL, BOTTOM_WALL, BACK_WALL);
	// 	glVertex3f(RIGHT_WALL, TOP_WALL, BACK_WALL);
	// 	glVertex3f(RIGHT_WALL, TOP_WALL, FRONT_WALL);
	// glEnd();
	// // BACK
	// glColor3d(0,1,1);
	// glBegin(GL_QUADS);
	// 	glVertex3f(LEFT_WALL, BOTTOM_WALL, BACK_WALL);
	// 	glVertex3f(RIGHT_WALL, BOTTOM_WALL, BACK_WALL);
	// 	glVertex3f(RIGHT_WALL, TOP_WALL, BACK_WALL);
	// 	glVertex3f(LEFT_WALL, TOP_WALL, BACK_WALL);
	// glEnd();
	// // TOP
	// glColor3d(0,0,1);
	// glBegin(GL_QUADS);
	// 	glVertex3f(LEFT_WALL, TOP_WALL, FRONT_WALL);
	// 	glVertex3f(RIGHT_WALL, TOP_WALL, FRONT_WALL);
	// 	glVertex3f(RIGHT_WALL, TOP_WALL, BACK_WALL);
	// 	glVertex3f(LEFT_WALL, TOP_WALL, BACK_WALL);
	// glEnd();

	for (int i = 0; i < NUM_PARTICLES; i++) {
		particles[i]->draw();
	}

	if (objmodel_ptr) {
		glPushMatrix();
		//glTranslatef(0,0,-11);
		glmDraw(objmodel_ptr, GLM_SMOOTH | GLM_MATERIAL);
		glPopMatrix();
	}
}
