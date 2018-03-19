#pragma once
#include <vector>
#include "../../Collision/Collision.h"
#include "../Object.h"

namespace NYLibrary
{
	class LocalObject : public virtual Object
	{
	public:
		LocalObject() { localSize = 0; }
		//�O�p�`�|���S����ݒ肷��
		void SetPolygons(const std::vector<Triangle>& polygons) { this->polygons = polygons; }
		//�O�p�`�|���S�����擾����
		const std::vector<Triangle>&GetPolygons() {return polygons;}
		//���[�J���̑傫����ݒ肷��
		void SetLocalSize(float localSize) { this->localSize = localSize; }
		//���[�J���̑傫�����擾����
		float GetLocalSize() { return localSize; }
	protected:
		//�O�p�`�|���S��
		std::vector<Triangle> polygons;
		//���[�J����̑傫��
		float localSize;
	};
}