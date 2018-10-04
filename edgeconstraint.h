#pragma once
#include "Constraint.h"

class EdgeConstraint : public Constraint
{
private:

	int		m_idx[2];  // To set indices for each projected vertices. To exploit parallalism
	float	m_weights[2];
	float	m_rest_dist;
	float	m_stiffness;
public:
	
	void solveLocal()
	{
		vec3	p0		= m_constrained_particles[0]->getPredictedPos();
		vec3	p1		= m_constrained_particles[1]->getPredictedPos();
		vec3	dir		= p0 - p1;
		float	mag		= glm::length(dir) - m_rest_dist;
		vec3	prj0	= p0 - m_weights[0] * mag *normalize(dir);
		vec3	prj1	= p1 + m_weights[1] * mag *normalize(dir);

		m_constrained_particles[0]->setProjectedPoint(m_stiffness*prj0, m_idx[0]);
		m_constrained_particles[1]->setProjectedPoint(m_stiffness*prj1, m_idx[1]);

	}

	EdgeConstraint(Particle* p0, Particle* p1, int idx0, int idx1, float weight0, float weight1, float stiffness , float rest_dist)
	{
		m_constrained_particles.push_back(p0);
		m_constrained_particles.push_back(p1);

		m_idx[0]		= idx0;
		m_idx[1]		= idx1;

		m_weights[0]	= weight0;
		m_weights[1]	= weight1;

		m_rest_dist		= rest_dist;
		m_stiffness		= stiffness;

	}
	ConstraintType getConstraintType()
	{
		return EDGE_CONSTRAINT;
	}

};