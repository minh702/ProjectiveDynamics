#pragma once
#include "glm/glm.hpp"
#include <vector>
#include "RenderObject.h"
using namespace glm;
using namespace std;

enum nodetype{ INTERNAL, LEAF };

typedef struct Node
{
	nodetype type;
	vec3 min, max;
	struct Node* children[2];

}Node;

Node* BottomUp(vector<RenderObject>& ros)
{
	vector<Node*> candidates;
	float vol=0;
	for (auto & ro : ros)
	{
		Node* leaf = new Node; 

		leaf->children[0] = NULL;
		leaf->children[1] = NULL;

		leaf->min = ro.min;
		leaf->max = ro.max;

		leaf->type = LEAF;

		candidates.push_back(leaf);
	}

	int pair0=0, pair1=0;
	float minVol=FLT_MAX;
	vec3 min, max;
	vec3 m, M;
	while (candidates.size() > 1)
	{
		minVol = FLT_MAX;
		pair0 = 0;
		pair1 = 0;
		min = vec3(0, 0, 0);
		max = vec3(0, 0, 0);
		for (int i = 0; i < candidates.size() - 1; i++)
		{
			for (int j = i+1; j < candidates.size(); j++)
			{
				for (int k = 0; k < 3; k++)
				{
					if (candidates[i]->min[k] < candidates[j]->min[k]) 
						min[k] = candidates[i]->min[k]; 
					else min[k] = candidates[j]->min[k]; //merge

					if (candidates[i]->max[k] > candidates[j]->max[k]) 
						max[k] = candidates[i]->max[k]; 
					else max[k] = candidates[j]->max[k];
				}

				vol = (max.x - min.x)*(max.y - min.y)*(max.z - min.z);
				if (minVol > vol)
				{
					minVol = vol;
					m = min;
					M = max;
					pair0 = i;
					pair1 = j;

				}
			}

		}

		Node* node = new Node;

		node->children[0] = candidates[pair0];
		node->children[1] = candidates[pair1];
		node->max = M;
		node->min = m;
		node->type = INTERNAL;
	
		candidates[pair0] = node;

		candidates[pair1] = candidates[candidates.size() - 1];
		candidates.pop_back();
		
	}

	return candidates[0];
}

void DeleteTree(Node* root)
{
	if (root != NULL)
	{
		if (root->children[0] != NULL)
		{
			DeleteTree(root->children[0]);
		}
		if (root->children[1] != NULL)
		{
			DeleteTree(root->children[1]);
		}
		
		delete root;
	}
}

void DrawBV(vec3 min, vec3 max,vec3 color)
{
	glBegin(GL_LINES); //1
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(max.x, min.y, min.z);
	glEnd();

	glBegin(GL_LINES); //2 
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glEnd();


	glBegin(GL_LINES); //3
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, min.y, min.z);
	glVertex3f(min.x, min.y, max.z);
	glEnd();

	glBegin(GL_LINES); //4
	glColor3f(color.r, color.g, color.b);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();

	glBegin(GL_LINES); //5
	glColor3f(color.r, color.g, color.b);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, min.y, max.z);
	glEnd();

	glBegin(GL_LINES); //6
	glColor3f(color.r, color.g, color.b);
	glVertex3f(max.x, max.y, max.z);
	glVertex3f(max.x, max.y, min.z);
	glEnd();

	glBegin(GL_LINES); //7
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glEnd();

	glBegin(GL_LINES); //8
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();

	glBegin(GL_LINES); //9 
	glColor3f(color.r, color.g, color.b);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(min.x, min.y, max.z);
	glEnd();


	glBegin(GL_LINES); //10
	glColor3f(color.r, color.g, color.b);
	glVertex3f(max.x, min.y, max.z);
	glVertex3f(max.x, min.y, min.z);
	glEnd();


	glBegin(GL_LINES); //11
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, max.y, min.z);
	glVertex3f(min.x, max.y, min.z);
	glEnd();

	glBegin(GL_LINES); // 12
	glColor3f(color.r, color.g, color.b);
	glVertex3f(min.x, min.y, max.z);
	glVertex3f(min.x, max.y, max.z);
	glEnd();

	glBegin(GL_LINES);
	glColor3f(color.r, color.g, color.b);
	glVertex3f(max.x, min.y, min.z);
	glVertex3f(max.x, max.y, min.z);
	glEnd();

}

void RenderCollided(vector<RenderObject>& ros)
{
	bool flags[3];
	for (int i = 0; i < ros.size() - 1; i++)
	{
		for (int j = i + 1; j < ros.size(); j++)
		{
			for (int k = 0; k < 3; k++)
			{
				if (ros[i].min[k] > ros[j].max[k] || ros[j].min[k] > ros[i].max[k])
				{
					flags[k] = true;
				}
				else
					flags[k] = false;
				
			}

			if (flags[0] && flags[1] && flags[2])
			{
				DrawBV(ros[i].min, ros[i].max, vec3(1, 0, 0));
				DrawBV(ros[j].min, ros[j].max, vec3(1, 0, 0));
			}

		}

	}

}

void renderTree(Node* root)
{
	if (root != NULL)
	{
		DrawBV(root->min, root->max,vec3(1,1,1));
		renderTree(root->children[0]);
		renderTree(root->children[1]);
	}
	
}


