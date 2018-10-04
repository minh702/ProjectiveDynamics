#pragma once
#include "Constraint.h"

class HingeConstraint : public Constraint
{
private :
	int		m_idx[5];
	float	m_rest_angle;
	float	m_stiffness;
public :
	HingeConstraint(Particle* p0, Particle* p1, Particle* p2, Particle* p3, Particle* p4, float rest_angle, float stiffness)
	{
		m_constrained_particles.push_back(p0);
		m_constrained_particles.push_back(p1);
		m_constrained_particles.push_back(p2);
		m_constrained_particles.push_back(p3);
		m_constrained_particles.push_back(p4);
		m_rest_angle	= rest_angle;
		m_stiffness		= stiffness;
	}

	void solveLocal()
	{

	}
};