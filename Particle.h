#pragma once

#include <glm.hpp>
#include <vector>
using namespace std;
using namespace glm;

class Particle
{
private:

	vec3		  m_pos;
	vec3		  m_vel;
	vec3		  m_pos_pred;
	vector<vec3>  m_pos_prj;
	vec3		  m_sn;
	vec3		  m_f_ext;
	vec3		  m_prj_sum;
	vec3          m_normal;
	float		  m_sum_stiff;
	float		  m_invMass;


public:

	vec3&	getPredictedPos()
	{
		return m_pos_pred;
	}
	int		getProjectedPosSize()
	{
		return m_pos_prj.size();
	}
	void	increaseProjectedPosSize()
	{
		m_pos_prj.push_back(vec3(0, 0, 0));
	}
	vec3&	getPos()
	{
		return m_pos;
	}
	void	setProjectedPoint(vec3& prj, int& idx)
	{
		m_pos_prj[idx] = prj;
	}
	void	applyForce(vec3 force)
	{
		m_f_ext += force;
	}
	void	predictMomentumPotential(float& timeStep)
	{
		m_sn		= m_pos + m_vel * timeStep + m_invMass * m_f_ext *timeStep*timeStep;
		m_pos_pred	= m_sn;
	}
	void	solveGlobal(float& timeStep)
	{
		m_prj_sum = vec3(0);
		for (unsigned int i = 0; i < m_pos_prj.size(); i++)
			m_prj_sum += m_pos_prj[i];

		m_pos_pred = (m_prj_sum + m_invMass * m_sn / (timeStep*timeStep)) / (m_sum_stiff + m_invMass / (timeStep*timeStep));

		vec3 tet = m_pos_pred - m_pos;

	}
	void	setinvMass(float& invMass)
	{
		m_invMass = invMass;
	}
	Particle(vec3& vtx)
	{
		m_pos		= vtx;
		m_vel		= vec3(0);
		m_pos_pred	= vec3(0);
		m_pos_prj.resize(0);
		m_sn		= vec3(0);
		m_f_ext		= vec3(0);
		m_sum_stiff = 0.f;
		m_invMass	= 1.f;
		m_prj_sum	= vec3(0);
	}
	void	increaseStiffSum(float stiff)
	{
		m_sum_stiff += stiff;
	}
	void	update_pos(float& timeStep)
	{

		m_vel	= (m_pos_pred - m_pos) / timeStep;
		m_pos	= m_pos_pred;
		m_f_ext	= vec3(0, 0, 0);
	}

};

