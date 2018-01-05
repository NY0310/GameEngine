#pragma once

#include <vector>


class AttributeMap
{
public:
	using TiledMap = std::vector<std::vector<int>>;
	//����
	enum Attribute
	{
		START = -3,	//�X�^�[�g
		END ,       //�S�[��
		WALL,       //��
		NONE,       //�����Ȃ�
		SWAMP		//��
	};
	//CSV�t�@�C���̓ǂݍ���
	void ReadCSV(std::string filename);
	//�`��
#pragma region Getter
	//�����}�b�v
	const int GetAttributeMap(int i, int j) const { return m_attributeMap[i][j]; }
	//�}�b�v�̍���
	const int GetMapHight    () const { return m_mapHight; }
	//�}�b�v�̕�
	const int GetMapWidth    () const { return m_mapWidth; }
	//�X�^�[�g�ʒu��X���W
	const int GetStartX      () const { return m_startX  ; }
	//�X�^�[�g�ʒu��Y���W
	const int GetStartY      () const { return m_startY  ; }
	//�S�[���ʒu��X���W
	const int GetEndX        () const { return m_endX    ; }
	//�S�[���ʒu��Y���W
	const int GetEndY        () const { return m_endY    ; }
#pragma endregion

private:
	// �����}�b�v
	TiledMap m_attributeMap;
	//�}�b�v�̍���
	int m_mapHight;
	//�}�b�v�̕�
	int m_mapWidth;
	//�X�^�[�g�ʒu��X���W
	int m_startX;
	//�X�^�[�g�ʒu��Y���W
	int m_startY;
	//�S�[���ʒu��X���W
	int m_endX;
	//�S�[���ʒu��X���W
	int m_endY;

	//1�s�̂ݎ擾
	void Get1Row(int& setNum, std::ifstream &ifs);
	//�c��̍s��ǂݍ���
	void ReadRestRow(std::ifstream &ifs);
	//����̈ʒu�����߂�
	void FindSpecificPos(int targetPos, int& posX, int& posY, int i ,const std::string& str);
};