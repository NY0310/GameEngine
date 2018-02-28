///// <summary>
///// ���f���G�t�F�N�g
///// </summary>
//#pragma once
//
//#include <d3d11_1.h>
//#include <SimpleMath.h>
//#include <list>
//#include "Obj3d.h"
//
//class ModelEffectManager
//{
//public:
//	using Vector3 = DirectX::SimpleMath::Vector3;
//
//	// �V���O���g���C���X�^���X�̎擾
//	static ModelEffectManager* getInstance();
//	// �j��
//	void Dispose();
//	// ����������
//	void Entry(const wchar_t*filename, int frame, Vector3 position, Vector3 velocity, Vector3 accel, Vector3 start_rotation, Vector3 end_rotation, Vector3 start_scale, Vector3 end_scale);
//	void Update();
//	void Draw();
//	void ChengMatrix();
//private:
//	ModelEffectManager();	
//
//	// �G�t�F�N�g1��
//	class ModelEffect
//	{
//	public:
//		ModelEffect()
//		{
//			frame = 0;
//			num_frame = 0;
//		}
//
//		bool Update(const Vector3& gravity);
//
//		// �I�u�W�F�N�g
//		Obj3d obj;
//		// ���W
//		Vector3 position;
//		// ���x
//		Vector3 velocity;
//		// �����x
//		Vector3 accel;
//		// �����l
//		Vector3 s_rotation;
//		Vector3 s_scale;
//		// �ŏI�l
//		Vector3 e_rotation;
//		Vector3 e_scale;
//		// ���݃t���[��
//		int   frame;
//		// �I���t���[��
//		int   num_frame;
//	};
//
//	// ���f���G�t�F�N�g�̔z��
//	std::list<ModelEffect> modelEffects;
//	// �d�͉����x
//	Vector3 gravity;
//};