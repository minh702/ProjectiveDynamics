#pragma once


#include "Constraint.h"
#include "Particle.h"
#include <omp.h>



class DeformableBody
{
private :
	int					m_max_itr;
	vec3				m_gravity;
	vector<Particle*>	m_particles;
	vector<Constraint*> m_constraints;


public:
	DeformableBody(int max_itr)
	{
		m_max_itr = max_itr;
	}
	
	~DeformableBody()
	{
		for (unsigned int i = 0; i < m_particles.size(); i++)
		{
			Particle* p = m_particles[i];
			m_particles[i] = 0;
			delete p;
		}

		for (unsigned int i = 0; i < m_constraints.size(); i++)
		{
			Constraint* c = m_constraints[i];
			m_constraints[i] = 0;
			delete c;
		}
	}
	void		addParticle(Particle* p)
	{
		m_particles.push_back(p);
	}
	int		getNumberOfParticles()
	{
		return m_particles.size();
	}
	void		setGravity(vec3 gravity)
	{
		m_gravity = gravity;
	}
	Particle*	getParticle(int idx)
	{
		return m_particles[idx];
	}
	void		addConstraint(Constraint* c)
	{
		m_constraints.push_back(c);
	}
	void		stepSimulation(float& timeStep)
	{

#pragma omp parallel
{

#pragma omp for
			for (int i = 0; i < m_particles.size(); i++)
			{
				m_particles[i]->applyForce(m_gravity);
				m_particles[i]->predictMomentumPotential(timeStep);
			}
}

		for (int i = 0; i < m_max_itr; i++)
		{
#pragma omp parallel
{
			#pragma omp for
			for (int j = 0; j < m_constraints.size(); j++)
				m_constraints[j]->solveLocal();

			#pragma omp for
			for (int j = 0; j < m_particles.size(); j++)
				m_particles[j]->solveGlobal(timeStep);

}
		}

#pragma omp parallel
{
			#pragma omp for
			for (int i = 0; i < m_particles.size(); i++)
				m_particles[i]->update_pos(timeStep);
}

	}
};