#pragma once
#include <vector>
#include "../../Collision/Collision.h"
#include "../Object.h"

namespace NYLibrary
{
	class LocalObject : public virtual Object
	{
	public:
		LocalObject() {}
		//�O�p�`�|���S����ݒ肷��
		void SetPolygons(std::vector<Triangle> polygons) { this->polygons = polygons; }
		//�O�p�`�|���S�����擾����
		std::vector<Triangle> GetPolygons() {return polygons;}
		//���[�J���̑傫����ݒ肷��
		void SetLocalSize(float localSize) { this->localSize.x = localSize; 
		this->localSize.y = localSize;
		this->localSize.z = localSize;
		
		}
		//���[�J���̑傫����ݒ肷��
		void SetLocalSizeX(float localSize) {
			this->localSize.x = localSize;
		}
		//���[�J���̑傫����ݒ肷��
		void SetLocalSizeY(float localSize) {
			this->localSize.y = localSize;
		}
		//���[�J���̑傫����ݒ肷��
		void SetLocalSizeZ(float localSize) {
			this->localSize.z = localSize;
		}
		//���[�J���̑傫����ݒ肷��
		void SetLocalSize(const D3DXVECTOR3& localSize) {
			this->localSize = localSize;
		}
		//���[�J���̑傫�����擾����
		const D3DXVECTOR3& GetLocalSize() { return localSize; }
	protected:
		//�O�p�`�|���S��
		std::vector<Triangle> polygons;
		//���[�J����̑傫��
		D3DXVECTOR3 localSize;
	};
}