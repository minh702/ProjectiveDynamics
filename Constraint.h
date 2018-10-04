#pragma once
#include "Particle.h"
			///< A triplet, used in the sparse triplet representation for matrices.
enum ConstraintType
{
	EDGE_CONSTRAINT,
	FIX_CONSTRAINT,
	HINGE_CONSTRAINT
	
};

class Constraint
{
protected:
	vector<Particle*> m_constrained_particles;

public:
	virtual void			solveLocal()=0;
	virtual ConstraintType	getConstraintType()=0;
};
