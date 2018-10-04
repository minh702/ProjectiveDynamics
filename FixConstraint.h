#pragma once
#include "Constraint.h"
class FixConstraint : public Constraint
{
private :
	vec3	m_pos_fixed;
	int		m_idx;
	float	m_stiffness;
public:
	void			solveLocal()
	{
		m_constrained_particles[0]->setProjectedPoint(m_stiffness*m_pos_fixed, m_idx);
	}
	ConstraintType	getConstraintType()
	{
		return FIX_CONSTRAINT;
	}

	FixConstraint(Particle* p, vec3& pos_fixed, float stiffness, int idx)
	{
		m_constrained_particles.push_back(p);
		m_pos_fixed = pos_fixed;
		m_idx		= idx;
		m_stiffness = stiffness;
	}
};