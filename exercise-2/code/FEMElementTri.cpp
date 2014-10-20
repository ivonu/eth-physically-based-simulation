/*=====================================================================================*/
/*! \file		FEMElementTri.cpp
	\author		peterkau
	\brief		Implementation of class FEMElementTri
 */
/*=====================================================================================*/

#include "SimpleFEMDefs.h"
#include <algorithm>
#include "FEMElementTri.h"
#include "FEMMesh.h"

#define computeSingleBasisDerivGlobalVar computeSingleBasisDerivGlobalLES

// TASK 3
void FEMElementTri::Assemble(FEMMesh *pMesh) const
{
	Vector2 node_1 = pMesh->GetNodePosition(m_nodes[0]);
	Vector2 node_2 = pMesh->GetNodePosition(m_nodes[1]);
	Vector2 node_3 = pMesh->GetNodePosition(m_nodes[2]);

	Vector2 middlePt((node_1[0] + node_2[0]) / 2, (node_1[1] + node_2[1]) / 2); double h = Vector2(node_3 - middlePt).length();
	float area = (node_1 - node_2).length() * (h/2);

	Vector2 node_1_grad; computeSingleBasisDerivGlobalVar(m_nodes[0], node_1_grad, pMesh);
	Vector2 node_2_grad; computeSingleBasisDerivGlobalVar(m_nodes[1], node_2_grad, pMesh);
	Vector2 node_3_grad; computeSingleBasisDerivGlobalVar(m_nodes[2], node_3_grad, pMesh);

	pMesh->AddToStiffnessMatrix(max(m_nodes[0], m_nodes[1]), min(m_nodes[0], m_nodes[1]), area * (node_1_grad | node_2_grad));
	pMesh->AddToStiffnessMatrix(max(m_nodes[1], m_nodes[2]), min(m_nodes[1], m_nodes[2]), area * (node_2_grad | node_3_grad));
	pMesh->AddToStiffnessMatrix(max(m_nodes[0], m_nodes[2]), min(m_nodes[0], m_nodes[2]), area * (node_1_grad | node_3_grad));
	
	pMesh->AddToStiffnessMatrix(m_nodes[0], m_nodes[0], area*(node_1_grad|node_1_grad));
	pMesh->AddToStiffnessMatrix(m_nodes[1], m_nodes[1], area*(node_2_grad | node_2_grad));
	pMesh->AddToStiffnessMatrix(m_nodes[2], m_nodes[2], area*(node_3_grad | node_3_grad));
}

// TASK 2
void FEMElementTri::computeSingleBasisDerivGlobalGeom(size_t nodeId, Vector2 &basisDerivGlobal, const FEMMesh *pMesh) const
{
	int loc_idx;
	if (nodeId == m_nodes[0]) {
		loc_idx = 0;
	} else if (nodeId == m_nodes[1]) {
		loc_idx = 1;
	} else if (nodeId == m_nodes[2]) {
		loc_idx = 2;
	}
	
	Vector2 node_1(pMesh->GetNodePosition(m_nodes[(loc_idx + 1) % 3]));
	Vector2 node_2(pMesh->GetNodePosition(m_nodes[(loc_idx + 2) % 3]));

	Vector2 current_node(pMesh->GetNodePosition(nodeId));

	Vector2 edge(node_2 - node_1);
	Vector2 normal(-edge[1], edge[0]); 

	Vector2 middlePt((node_1[0] +node_2[0])/2, (node_1[1] + node_2[1]) / 2);
	double h = Vector2(current_node - middlePt).length();
	basisDerivGlobal = normal.normalized() / h;

	//cout << "(Geometry) Node Id = " << nodeId << ": " << basisDerivGlobal << endl;
	
}

// TASK 1
void FEMElementTri::computeSingleBasisDerivGlobalLES(size_t nodeId, Vector2 &basisDerivGlobal, const FEMMesh *pMesh) const
{
	Vector2 node_pos1 = pMesh->GetNodePosition(m_nodes[0]);
	Vector2 node_pos2 = pMesh->GetNodePosition(m_nodes[1]);
	Vector2 node_pos3 = pMesh->GetNodePosition(m_nodes[2]);

	int delta1 = (nodeId == m_nodes[0]) ? 1 : 0;
	int delta2 = (nodeId == m_nodes[1]) ? 1 : 0;
	int delta3 = (nodeId == m_nodes[2]) ? 1 : 0;
	Vector3d deltaV(delta1, delta2, delta3);

	Matrix3x3d M; 
	M(0, 0) = node_pos1[0]; M(0, 1) = node_pos1[1]; M(0, 2) = 1;
	M(1, 0) = node_pos2[0]; M(1, 1) = node_pos2[1]; M(1, 2) = 1;
	M(2, 0) = node_pos3[0]; M(2, 1) = node_pos3[1]; M(2, 2) = 1;

	Vector3 res(M.inverse()*deltaV);
	
	basisDerivGlobal = Vector2(res[0], res[1]);
	//cout << "(Normal)  Node Id = " << nodeId << ": " << basisDerivGlobal << endl;
}

