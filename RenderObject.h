#pragma once
#include <vector>
#include <iostream>
#include <string>
#include <fstream>
#include "ProjectiveDynamics\DeformableBody.h"
#include "ProjectiveDynamics\EdgeConstraint.h"
#include "ProjectiveDynamics\FixConstraint.h"


float g_stiffness = 55000.f;
struct Face
{
	int m_fID[3];
	Face(int v0, int v1, int v2)
	{
		m_fID[0] = v0;
		m_fID[1] = v1;
		m_fID[2] = v2;
	}


};
struct Edge
{
	int m_eID[2];
	Edge(int v0, int v1)
	{
		m_eID[0] = v0;
		m_eID[1] = v1;
	}

};
struct adjacentFaces
{
	unsigned int m_fID[2];
	adjacentFaces(int idx0, int idx1)
	{
		m_fID[0] = idx0;
		m_fID[1] = idx1;
	}
};

class RenderObject
{
private:
	DeformableBody* m_db;
	vector<vec3> m_vtx;
	void update_vtx()
	{
		for (int i = 0; i < m_db->getNumberOfParticles(); i++)
		{
			m_vtx[i] = m_db->getParticle(i)->getPos();
		}
	}
	void vtx2Particle()
	{
		for (unsigned int i = 0; i < m_vtx.size(); i++)
		{
			Particle * p = new Particle(m_vtx[i]);
			m_db->addParticle(p);
		}
	}
	void Edge2EdgeConstraint()
	{
		for (unsigned int i = 0; i < m_e.size(); i++)
		{
			Particle * p1	= m_db->getParticle(m_e[i].m_eID[0]);
			Particle * p2	= m_db->getParticle(m_e[i].m_eID[1]);	
			float length	= glm::length(p1->getPos() - p2->getPos());
			int idx0		= p1->getProjectedPosSize();
			int idx1		= p2->getProjectedPosSize();

			p1->increaseProjectedPosSize();
			p2->increaseProjectedPosSize();
			p1->increaseStiffSum(g_stiffness);
			p2->increaseStiffSum(g_stiffness);
			EdgeConstraint *ec = new EdgeConstraint(p1, p2, idx0, idx1, 0.5, 0.5, g_stiffness, length);
			m_db->addConstraint(ec);
			
		}
	}
	void getAllEgdes()
	{
		bool flag;
		for (unsigned int i = 0; i < m_f.size(); i++)
		{
			unsigned int j;
			//v0v1
			flag = true;
			for (j = 0; j < m_e.size(); j++)
			{
				if ((m_e[j].m_eID[0] == m_f[i].m_fID[0] && m_e[j].m_eID[1] == m_f[i].m_fID[1]) || (m_e[j].m_eID[1] == m_f[i].m_fID[0] && m_e[j].m_eID[1] == m_f[i].m_fID[1]))
				{
					flag = false;
					break;
				}
			}

			if (flag)
			{
				m_e.push_back(Edge(m_f[i].m_fID[0], m_f[i].m_fID[1]));
			}
			//v1v2
			flag = true;
			for (j = 0; j < m_e.size(); j++)
			{
				if ((m_e[j].m_eID[0] == m_f[i].m_fID[1] && m_e[j].m_eID[1] == m_f[i].m_fID[2])|| (m_e[j].m_eID[1] == m_f[i].m_fID[1] && m_e[j].m_eID[0] == m_f[i].m_fID[2]))
				{
					flag = false;
					break;
				}
			}

			if (flag)
			{
				m_e.push_back(Edge(m_f[i].m_fID[1], m_f[i].m_fID[2]));
			}
			//v2v0
			flag = true;
			for (j = 0; j < m_e.size(); j++)
			{
				if ((m_e[j].m_eID[0] == m_f[i].m_fID[2] && m_e[j].m_eID[1] == m_f[i].m_fID[0])|| (m_e[j].m_eID[1] == m_f[i].m_fID[0] && m_e[j].m_eID[0] == m_f[i].m_fID[0]))
				{
					flag = false;
					break;
				}
			}

			if (flag)
			{
				m_e.push_back(Edge(m_f[i].m_fID[2], m_f[i].m_fID[0]));
			}


		}
	}
	void getAlladjacentFaces()
	{

	}

public :

	vector<Face> m_f;
	vector<Edge> m_e;
	void ImportFromObj(const std::string path)
	{
		fstream file(path);
		double x, y, z;
		vec3 center;
		int idx[3];
		char*  v[3];
		if (file)
		{
			string line;
			while (getline(file, line))
			{
				char * toChar = new char[line.size() + 1];

				strcpy(toChar, line.c_str());

				toChar[line.size()] = '\0';

				char * p = NULL;
				switch (toChar[0])

				{
				case 'v':


					if (toChar[1] != 'n')
					{
						x = atof(strtok(&toChar[1], " "));
						y = atof(strtok(NULL, " "));
						z = atof(strtok(NULL, " "));

						m_vtx.push_back(vec3(x, y, z));
					}

					break;

				case'f':


					v[0] = strtok(&toChar[1], " ");
					v[1] = strtok(NULL, " ");
					v[2] = strtok(NULL, " ");

					idx[0] = atoi(strtok(v[0], "/")) - 1;
					idx[1] = atoi(strtok(v[1], "/")) - 1;
					idx[2] = atoi(strtok(v[2], "/")) - 1;
					m_f.push_back(Face(idx[0], idx[1], idx[2]));


					break;
				}

				delete[] toChar;

			}
		}
		else
			cout << "there is no such file...\n" << endl;
		


		vtx2Particle();
		getAllEgdes();
		Edge2EdgeConstraint();


		float stiffness_fix = 1000000;
		{
			Particle * p	= m_db->getParticle(240);
			vec3 fixed_vtx	= p->getPos();
			int idx			= p->getProjectedPosSize();
			p->increaseProjectedPosSize();
			p->increaseStiffSum(stiffness_fix);
			FixConstraint * fc = new FixConstraint(p, fixed_vtx, stiffness_fix, idx);
			m_db->addConstraint(fc);	
		}
		

		{
			Particle * p = m_db->getParticle(255);
			vec3 fixed_vtx = p->getPos();
			int idx = p->getProjectedPosSize();
			p->increaseProjectedPosSize();
			p->increaseStiffSum(stiffness_fix);
			FixConstraint * fc = new FixConstraint(p, fixed_vtx, stiffness_fix, idx);
			m_db->addConstraint(fc);
		}

		{
			Particle * p = m_db->getParticle(1);
			vec3 fixed_vtx = p->getPos();
			int idx = p->getProjectedPosSize();
			p->increaseProjectedPosSize();
			p->increaseStiffSum(stiffness_fix);
			FixConstraint * fc = new FixConstraint(p, fixed_vtx, stiffness_fix, idx);
			m_db->addConstraint(fc);
		}

		{
			Particle * p = m_db->getParticle(31);
			vec3 fixed_vtx = p->getPos();
			int idx = p->getProjectedPosSize();
			p->increaseProjectedPosSize();
			p->increaseStiffSum(stiffness_fix);
			FixConstraint * fc = new FixConstraint(p, fixed_vtx, stiffness_fix, idx);
			m_db->addConstraint(fc);
		}

	}


	void stepSimulation(float timeStep)
	{
		m_db->stepSimulation(timeStep);
		update_vtx();
	}
	void DrawFaces()
	{
		glBegin(GL_TRIANGLES);
		for (auto & f : m_f)
		{
			glVertex3f(m_vtx[f.m_fID[0]].x, m_vtx[f.m_fID[0]].y, m_vtx[f.m_fID[0]].z);
			glVertex3f(m_vtx[f.m_fID[1]].x, m_vtx[f.m_fID[1]].y, m_vtx[f.m_fID[1]].z);
			glVertex3f(m_vtx[f.m_fID[2]].x, m_vtx[f.m_fID[2]].y, m_vtx[f.m_fID[2]].z);
		}
		glEnd();

	}

	void DrawWireFrame()
	{
		
		for (auto &e : m_e)
		{
			glBegin(GL_LINES);
			glColor3f(1, 1, 1);

			glVertex3f(m_vtx[e.m_eID[0]].x, m_vtx[e.m_eID[0]].y, m_vtx[e.m_eID[0]].z);
			glVertex3f(m_vtx[e.m_eID[1]].x, m_vtx[e.m_eID[1]].y, m_vtx[e.m_eID[1]].z);

			glEnd();
		}



	}
	RenderObject(const string path)
	{
		m_db = new DeformableBody(15);
		m_db->setGravity(vec3(0,-10,0));
		ImportFromObj(path);
		
	}
	RenderObject()
	{

	}
};