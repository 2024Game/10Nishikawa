#ifndef CSHADOWSHADER_H
#define CSHADOWSHADER_H

#include "CMyShader.h"

class CShadowShader : public CMyShader
{
	//•`‰æˆ—
	void Render(const CModelX* model, const CMesh* mesh, const CMatrix* pCombinedMatrix);
public:
	//•`‰æˆ—
	void Render(CModelX* model, CMatrix* combinedMatrix);
	void Render330(const CMesh* mesh, const CMatrix* skin_matrix, int skin_matrix_size);
	void Render330(const CModel* model, const CMatrix* skin_matrix, int skin_matrix_size);
	void Render330(const GLuint myVertexBufferId, const std::vector<CMaterial*>& material,
		const std::vector<int>& materialVertexCount, const CMatrix* skin_matrix, const int skin_matrix_size);
	void SetShader(CMaterial* material);
};

#endif
