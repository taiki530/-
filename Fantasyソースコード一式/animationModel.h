#pragma once

#include <unordered_map>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "assimp/matrix4x4.h"
#pragma comment (lib, "assimp-vc143-mt.lib")

#include "component.h"


//�ό`�㒸�_�\����
struct DEFORM_VERTEX
{
	aiVector3D Position;
	aiQuaternion Rotation;
	aiVector3D Normal;
	int				BoneNum;
	std::string		BoneName[4];
	float			BoneWeight[4];
};

//�{�[���\����
struct BONE
{
	aiMatrix4x4 Matrix;
	aiMatrix4x4 AnimationMatrix;
	aiMatrix4x4 OffsetMatrix;
};

class AnimationModel : public Component
{
private:
	const aiScene* m_AiScene = nullptr;
	std::unordered_map<std::string, const aiScene*> m_Animation;

	ID3D11Buffer**	m_VertexBuffer;
	ID3D11Buffer**	m_IndexBuffer;

	std::unordered_map<std::string, ID3D11ShaderResourceView*> m_Texture;

	std::vector<DEFORM_VERTEX>* m_DeformVertex;//�ό`�㒸�_�f�[�^
	std::unordered_map<std::string, BONE> m_Bone;//�{�[���f�[�^�i���O�ŎQ�Ɓj

public:
	using Component::Component;

	void Load( const char *FileName );
	void LoadAnimation(const char* FileName, const char* Name);
	void CreateBone(aiNode* node);
	void UpdateBoneMatrix(aiNode* node, aiMatrix4x4 matrix);
	void Update(const char* AnimationName1, int Frame1,
				const char* AnimationName2, int Frame2,
				float BlendRatio);
	void Uninit() override;
	void Draw() override;
	XMMATRIX GetBoneMatrix(const std::string& boneName);
	XMFLOAT3 GetBoneWorldPosition(const std::string& boneName);
};
