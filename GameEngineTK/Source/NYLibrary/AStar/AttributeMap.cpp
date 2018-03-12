#include "AttributeMap.h"

#include <sstream>
#include <fstream>
#include <cassert>

using namespace std;
using namespace NYLibrary;

/// <summary>
/// CSV�t�@�C���̓ǂݍ���
/// </summary>
/// <param name="filename">�t�@�C����</param>
void AttributeMap::ReadCSV(std::string filename)
{
	ifstream ifs(filename);
	//�t�@�C�����Ȃ���assert
	assert(ifs);

	//�s�����擾
	Get1Row(m_mapHight, ifs);
	//�񐔂��擾
	Get1Row(m_mapWidth, ifs);

	//�����}�b�v�̃T�C�Y��ύX����
	m_attributeMap.resize(m_mapHight);
	for (auto& i : m_attributeMap)
		i.resize(m_mapWidth);
	//�c��̍s��ǂݍ���
	ReadRestRow(ifs);
}



/// <summary>
/// 1�s�̂ݎ擾
/// </summary>
/// <param name="setNum">�ύX�������l</param>
/// <param name="ifs">�X�g���[��</param>
/// <param name="str">������</param>
void AttributeMap::Get1Row(int& setNum, std::ifstream &ifs)
{
	string str;
	getline(ifs, str);
	setNum = atoi(str.c_str());
}

/// <summary>
/// �c��̍s��ǂݍ���
/// </summary>
/// <param name="ifs">�X�g���[��</param>
/// <param name="str">������</param>
void AttributeMap::ReadRestRow(std::ifstream & ifs)
{

	string str;
	int i = 0;
	//csv�t�@�C����1�s���ǂݍ���
	while (getline(ifs, str))
	{
		string token;
		istringstream stream(str);

		//1�s�̂����A������ƃR���}�𕪊�����
		while (getline(stream, token, ','))
		{
			//�X�^�[�g�̈ʒu�����߂�
			FindSpecificPos(START, m_startX, m_startY, i, token);
			//�I���̈ʒu�����߂�
			FindSpecificPos(END, m_endX, m_endY, i, token);
			//�}�b�v��csv�̏�����
			m_attributeMap[i / m_mapHight][i % m_mapWidth] = atoi(token.c_str());
			i++;
		}
	}
}

/// <summary>
/// ����̈ʒu�����߂�
/// </summary>
/// <param name="targetPos">���߂����ꏊ</param>
/// <param name="posX">�������X���W</param>
/// <param name="posY">�������Y���W</param>
/// <param name="i">���Ԗ�</param>
/// <param name="str">������</param>
void AttributeMap::FindSpecificPos(int targetPos, int& posX, int& posY, int i , const std::string& str)
{
	if (atoi(str.c_str()) == targetPos)
	{
		posX = i / m_mapHight;
		posY = i % m_mapWidth;
	}
}
