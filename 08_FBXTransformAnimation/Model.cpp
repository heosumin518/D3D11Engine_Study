#include "pch.h"
#include "Model.h"
#include "Node.h"
#include "Mesh.h"
#include "Material.h"
#include "Animation.h"
#include "ModelLoader.h"

Model::Model()
{
	m_animation = make_shared<Animation>();
	m_rootNode = make_shared<Node>();
}

Model::~Model()
{
	
}

void Model::Update(float deltaTime)
{
	m_rootNode->Update(deltaTime);
}
