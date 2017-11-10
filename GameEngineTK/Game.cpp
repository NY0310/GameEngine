//
// Game.cpp
//

//#pragma comment(lib,"d3dx10.lib")
//#pragma comment(lib,"d3dx11.lib")

#include "Game.h"
#include <ctime>

extern void ExitGame();

using namespace DirectX;
using namespace DirectX::SimpleMath;
using namespace std;
using Microsoft::WRL::ComPtr;




Game::Game(): 
	m_window(0),
	m_outputWidth(800),
	m_outputHeight(600),
	m_featureLevel(D3D_FEATURE_LEVEL_9_1)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND window, int width, int height)
{
	
	auto& devices = Devices::Get();
	devices.HWnd(window);
	devices.Width(width);
	devices.Height(height);

	// �f�o�C�X�𐶐����� Create Device
	devices.CreateDevice();
	// ���\�[�X�𐶐����� Create Resources
	devices.CreateResources();



	srand(static_cast<unsigned int>(time(nullptr)));

	m_window = window;
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);


	//	�������͂����ɏ���
	//	�L�[�{�[�h�̐���
	keyboard = std::make_unique<Keyboard>();
	//	�J�����̐���
	m_Camera = std::make_unique<FollowCamera>(m_outputWidth, m_outputHeight);
	//	�J�����ɃL�[�{�[�h���Z�b�g
	m_Camera->SetKeyboard(keyboard.get());
	m_spriteFont = std::make_unique<SpriteFont>(devices.Device().Get(), L"Resources/myfile.spritefont");


	{//OBj3d�̃V�X�e��������
		Obj3d::CommonDef def;
		def.pCamera = m_Camera.get();
		def.pDevice = devices.Device().Get();
		def.pDeviceContext = devices.Context().Get();
	 //	3D�I�u�W�F�N�g�̐ÓI�����o�ϐ���������
		Obj3d::InitializeCommon(def);
	}
	

	//�y�n�����蔻��
	LandShapeCommonDef lscDef;
	lscDef.pDevice = devices.Device().Get();
	lscDef.pDeviceContext = devices.Context().Get();
	lscDef.pCamera = m_Camera.get();
	//�y�n�̂����蔻�苤�ʏ�����
	LandShape::InitializeCommon(lscDef);


	m_LandShape.Initialize(L"Skydome2", L"Skydome2");
	m_LandShape.SetScale(1.1);
	
	m_batch = std::make_unique<PrimitiveBatch<VertexPositionNormal>>(devices.Context().Get());



	/*void const* shaderByteCode;
	size_t byteCodeLength;*/


	//	�ėp�X�e�[�g�𐶐�	q
	m_states = std::make_unique<CommonStates>(devices.Device().Get());
	
	devices.Context().Get()->RSSetState(m_states->CullClockwise());


	//	�f�o�b�O�J�����𐶐�
	m_debugCamera = std::make_unique<DebugCamera>(m_outputWidth, m_outputHeight);

	//	�G�t�F�N�g�t�@�N�g���̍쐬
	m_factory = std::make_unique<EffectFactory>(devices.Device().Get());
	//	�e�N�X�`���̃p�X���w��
	m_factory->SetDirectory(L"Resources");
	//	�V�����f���̐���
//	m_objSkydome.LoadModel(L"Skydome.cmo");
	


	player = std::make_unique<Player>(keyboard.get());
	player->Initialize();
	//�Ǐ]�J�����Ƀv���C���[���Z�b�g
	m_Camera->SetPlayer(player.get());

	//�G�̐���
	int enemyNum = 5;

	m_Enemies.resize(enemyNum);

	for (int i = 0; i < enemyNum; i++)
	{
		m_Enemies[i] = std::make_unique<Enemy>(keyboard.get());
		//m_Enemies[i]->Initialize();
	}


	clearcnt = 0;
	

	m_str = L"CLEAR";

	stage.Initialize();

	tomanageparticle = new ToManageParticle();
	tomanageparticle->Init();

	obj = new OBJ();
	obj->Init();

	shadermanager = ShaderManager::Get();


	//D3DXMESH���C�u�������g�p����N���X����
	m_pMesh = new CD3DXMESH;
	//������
	m_pMesh->Init("RobotA_1motion_2truck.x");


	pSss = new sss();
	pSss->InitD3D();


	pBumpMapping = new BumpMapping();
	pBumpMapping->InitD3D();


	pTessellation = new Tessellation();
	pTessellation->InitD3D();




	pDisplacementMapping = new DisplacementMapping();
	pDisplacementMapping->InitD3D();

	//D3D11_RASTERIZER_DESC rdc;
	//ZeroMemory(&rdc, sizeof(rdc));
	//rdc.CullMode = D3D11_CULL_NONE;
	//rdc.FillMode = D3D11_FILL_WIREFRAME;
	//rdc.FrontCounterClockwise = TRUE;
	//ID3D11RasterizerState* m_pRasterizerState;

	//devices.Device().Get()->CreateRasterizerState(&rdc, &m_pRasterizerState);
	//devices.Context().Get()->RSSetState(m_pRasterizerState);

}



// Executes the basic game loop.
void Game::Tick()
{
	m_timer.Tick([&]()
	{
		Update(m_timer);
	});

	Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& timer)
{

	// TODO: Add your game logic here.


	//	���t���[������������
	m_debugCamera->Update();

	stage.GetLandShape().Update();

	//	�L�[�{�[�h�̎擾
	Keyboard::State key = keyboard->GetState();


	////�L�[�{�[�h�̗l�Ԃ��擾
	Keyboard::State kb = keyboard->GetState();
	//�v���C���̃A�b�v�f�[�g�֐�
	player->Update();

	////�L�[�{�[�h�g���b�J�[�̍X�V
	KeybordTracker.Update(kb);


	m_LandShape.Update();

	if (player->Gethitcnt() == player->GetMAX_HOMING())
	{
		if (key.IsKeyDown(Keyboard::Keys::R))
		{
			player->ResetCnt();
			FireHomingBullets(player->GetTrans());
		}
	}

	{//�e�ۂƓG�̂����蔻��
		for (std::vector<SphereNode*>::iterator Sphereit = player->GetCollisionNodeBullet().begin(); Sphereit != player->GetCollisionNodeBullet().end(); )
		{
			if (m_Enemies.size() == 0)
			{
				break;
			}
			for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); )
			{

				Enemy* enemy = it->get();
				//�G�̔��苅���擾
				const Sphere& enemySphere = enemy->GetCollisionNodeBody();

				//��̋����������Ă�����
				if (CheckSpere2Sphere(*(*Sphereit), enemySphere))
				{
					(*it)->SetHp((*it)->GetHp() - 10);

					//���������e������
					Sphereit = player->GetCollisionNodeBullet().erase(Sphereit);
			

					if(player->Gethitcnt() != player->GetMAX_HOMING())
					player->Sethitcnt(player->Gethitcnt() + 1);
					//hp���Ȃ��Ȃ�����G���E��
					if ((*it)->GetHp() <= 0)
					{
						//�p�[�e�B�N��
						Vector3 enemypos = (*it)->GetTrans();
						enemypos.y += 1;
						tomanageparticle->AddParticle(enemypos);


						//�폜����G���^�[�Q�b�g�ɂ��Ă���z�[�~���O�e���폜	
						for (std::vector<std::unique_ptr<HomingBullet>>::iterator ithoming = m_HomingBullets.begin();
							ithoming != m_HomingBullets.end();)
						{
							if ((*it).get() == (*ithoming)->GetEnemy())
							{
								ithoming = m_HomingBullets.erase(ithoming);
							}
							else
							{
								ithoming++;
							}
						}

						it = m_Enemies.erase(it);
		
			
					
						m_Enemies.push_back(move(std::make_unique<Enemy>(keyboard.get())));
						clearcnt++;
					}
					break;
					
				}
				else
				{
					//�����Ȃ������̂�
					it++;
					//���ɂ�������Ȃ������̂ŏ���
					if (m_Enemies.end() == it)
					{
							//�����������̂Ń��[�v���甲����
  							Sphereit++;
					}
				}
			}
		}

	}




	//{//�G�e�ۂƃv���C���̂����蔻��
	//	for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end();it++)
	//	{
	//		for (std::vector<SphereNode*>::iterator Sphereit = (*it)->GetCollisionNodeBullet().begin(); Sphereit != (*it)->GetCollisionNodeBullet().end(); )
	//		{
	//				//�G�̔��苅���擾
	//				const Sphere& playerSphere = player->GetCollisionNodeBody();

	//				//��̋����������Ă�����
	//				if (CheckSpere2Sphere(*(*Sphereit), playerSphere))

	//				{
	//					player->SetHp(player->GetHp() - 10);

	//					//���������e������
	//					Sphereit = (*it)->GetCollisionNodeBullet().erase(Sphereit);
	//			
	//					//hp���Ȃ��Ȃ�����G���E��
	//					if (player->GetHp() <= 0)
	//					{
	//						//�p�[�e�B�N��
	//						Vector3 playerpos = player->GetTrans();
	//						playerpos.y += 1;
	//						tomanageparticle->AddParticle(playerpos);
	//					}

	//				}
	//				else
	//				{
	//						//�����������̂Ń��[�v���甲����
	//						Sphereit++;
	//				}

	//		}

	//	}

	//}






	
	//
	{//	���@�ɒǏ]����J����
		
		m_Camera->Update();
		m_view = m_Camera->GetView();
		m_proj = m_Camera->GetProjection();
	}

	

	const Vector3 vel = player->GetVelocity();
	//if (vel.y <= 0)
	//{//���@���n�ʂɏ�鏈��
	// //���@�̓����瑫���ւ̐���
	//	Segment player_segment;
	//	//���@�̃��[���h���W
	//	Vector3 trans = player->GetTrans();
	//	player_segment.Start = trans + Vector3(0, 1, 0);
	//	player_segment.End = trans + Vector3(0, -0.5f, 0);

	//	//��_���W
	//	Vector3 inter;

	//	//�n�`�Ɛ����̂����蔻��(���C�L���X�g Ray)
	//	if (stage.GetLandShape().IntersectSegment(player_segment, &inter))
	//	{
	//		trans.y = inter.y;
	//		//�������I��
	//		player->StopJump();
	//	}
	//	else
	//	{
	//		player->StartFall();
	//	}

	//	//���@���ړ�
	//	player->SetTrans(trans);
	//	//���[���h�s��̍X�V
	////	player->Calc();
	//}



	////���@�̓V���ւ̂߂肱�݂���������
	//{
	//	Sphere sphere = player->GetCollisionNodeBody();

	//	//���@�̃��[���h���W
	//	Vector3 trans = player->GetTrans();

	//	Vector3 sphere2player = trans - sphere.Center;
	//	//�߂肱�ݔr�˃x�N�g��
	//	Vector3 reject;

	//	if (m_LandShape.IntersectSphere(sphere, &reject))
	//	{
	//		sphere.Center += reject;

	//	}
	//	
	//		player->SetTrans(sphere.Center + sphere2player);



	//}


	//�G�̓V���ւ̂߂肱�݂���������

	//for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)

	//{
	//	Vector3 pos = (*it)->GetTrans();
	//	pos.x = abs(pos.x);
	//	pos.y = abs(pos.y);
	//	pos.z = abs(pos.z);

	//	if ((pos.x >= 75) || (pos.z >= 75))
	//	{

	//		//���@�̓V���ւ̂߂肱�݂���������
	//		{
	//			Sphere sphere = (*it)->GetCollisionNodeBody();

	//			//���@�̃��[���h���W
	//			Vector3 trans = (*it)->GetTrans();

	//			Vector3 sphere2player = trans - sphere.Center;
	//			//�߂肱�ݔr�˃x�N�g��
	//			Vector3 reject;

	//			if (m_LandShape.IntersectSphere(sphere, &reject))
	//			{
	//				
	//				
	//				//�폜����G���^�[�Q�b�g�ɂ��Ă���z�[�~���O�e���폜	
	//				for (std::vector<std::unique_ptr<HomingBullet>>::iterator ithoming = m_HomingBullets.begin();
	//					ithoming != m_HomingBullets.end();)
	//				{
	//					if ((*it).get() == (*ithoming)->GetEnemy())
	//					{
	//						ithoming = m_HomingBullets.erase(ithoming);
	//					}
	//					else
	//					{
	//						ithoming++;
	//					}
	//				}
	//				it = m_Enemies.erase(it);
	//				m_Enemies.push_back(move(std::make_unique<Enemy>(keyboard.get())));

	//			}
	//			else
	//			{
	//				//it++;
	//			}


	//		}


	//	}
	//	else
	//	{
	//	//	it++;
	//	}
	//}



	for (std::vector<std::unique_ptr<HomingBullet>>::iterator it = m_HomingBullets.begin();
		it != m_HomingBullets.end();
		)
	{
		if ((*it)->Update())
		{
			
			it = m_HomingBullets.erase(it);

		}
		else
		{
			it++;
		}
	}
	

	// �S�p�[�c���s��X�V
	//for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	//{
	//	Enemy* enemy = it->get();
	//	(*it)->Update();
	//}

	//�p�[�e�B�N���X�V����
	tomanageparticle->Update();

}

// Draws the scene.
void Game::Render()
{

	auto& devices = Devices::Get();
	


	// Don't try to render anything before the first Update.
	if (m_timer.GetFrameCount() == 0)
	{
		return;
	}




//#if 0
//�A���t�@�l��L���ɂ���
devices.Context().Get()->OMSetBlendState(m_states->Opaque(),nullptr,0xffffffff);

	//pSss->ZTexRender(m_Camera);

	Clear();
	//pBumpMapping->Render(m_Camera);


	//pSss->Render(m_Camera);


	//for (std::vector<std::unique_ptr<HomingBullet>>::iterator it = m_HomingBullets.begin();
	//	it != m_HomingBullets.end();
	//	it++)
	//{
	//	(*it)->Draw();
	//}


	//stage.GetLandShape().Draw();

	//m_LandShape.Draw();
	//VertexPositionColor v1(Vector3(0.f, 0.5f, 0.5f), Colors::SkyBlue);
	//VertexPositionColor v2(Vector3(0.5f, -0.5f, 0.5f), Colors::SandyBrown);
	//VertexPositionColor v3(Vector3(-0.5f, -0.5f, 0.5f), Colors::PaleGoldenrod);

	//CommonStates states(devices.Device().Get());

	//devices.SpriteBatch()->Begin(SpriteSortMode_Deferred, states.NonPremultiplied());


	//// �S�p�[�c���s��X�V
	//for (std::vector<std::unique_ptr<Enemy>>::iterator it = m_Enemies.begin(); it != m_Enemies.end(); it++)
	//{
	//	Enemy* enemy = it->get();
	//	(*it)->Draw();
	//}

	//player->Draw();

	//if (clearcnt == CLEARNUM)
	//{
	//	m_Enemies.clear();
	//	//�X�v���C�g�t�H���g�̕`��
 // 		m_spriteFont->DrawString(devices.SpriteBatch().get(), m_str.c_str(), XMFLOAT2(200, 200));

	//}

	//tomanageparticle->Render(m_Camera);
	//obj->Render(m_Camera);


	////�L�[�{�[�h�̗l�Ԃ��擾
	//Keyboard::State kb = keyboard->GetState();
	////�L�[�{�[�h�g���b�J�[�̍X�V
	//KeybordTracker.Update(kb);

	//if (kb.D)
	//{
	//	m_pMesh->ChangeAnimSet(1);
	//}





	//devices.SpriteBatch().get()->End();
	//�A���t�@�l�𖳌��ɂ���
	devices.Context().Get()->OMSetBlendState(m_states->Opaque(),nullptr,0xffffffff);
	//pTessellation->Render(m_Camera);

	////D3DXMESH���C�u�������g�p����X�t�@�C����`�悷��N���X
	//m_pMesh->Render(m_Camera, D3DXVECTOR3(1, 1, -1));
	//m_pMesh->GetfYaw() += 0.0002;
	//m_pMesh->GetAnimController()->AdvanceTime(0.007, NULL);


	pDisplacementMapping->Render(m_Camera);



	Present();
}
//
//// Helper method to clear the back buffers.
void Game::Clear()
{
	auto& devices = Devices::Get();


	// Clear the views.
	devices.Context().Get()->OMSetRenderTargets(1, devices.RenderTargetView().GetAddressOf(), devices.DepthStencilView().Get());
	devices.Context().Get()->ClearRenderTargetView(devices.RenderTargetView().Get(), Colors::CornflowerBlue);
	devices.Context().Get()->ClearDepthStencilView(devices.DepthStencilView().Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	// Set the viewport.
	CD3D11_VIEWPORT  viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
	devices.Context().Get()->RSSetViewports(1, &viewport);

	devices.Viewport(viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
	// The first argument instructs DXGI to block until VSync, putting the application
	// to sleep until the next VSync. This ensures we don't waste any cycles rendering
	// frames that will never be displayed to the screen.
	auto& devices = Devices::Get();
	HRESULT hr = devices.SwapChain().Get()->Present(1, 0);

	// If the device was reset we must completely reinitialize the renderer.
	if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
	{
		devices.OnDeviceLost();
	}
	else
	{
		DX::ThrowIfFailed(hr);
	}
}

// Message handlers
void Game::OnActivated()
{
	// TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
	// TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
	// TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
	m_timer.ResetElapsedTime();

	// TODO: Game is being power-resumed (or returning from minimize).
}
//
void Game::OnWindowSizeChanged(int width, int height)
{
	m_outputWidth = max(width, 1);
	m_outputHeight = max(height, 1);

	Devices::Get().CreateResources();

	// TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& width, int& height) const
{
	// TODO: Change to desired default window size (note minimum size is 320x200).
	width = 800;
	height = 600;
}




void Game::FireHomingBullets(const DirectX::SimpleMath::Vector3 pos)
{
	// �G�̐�����������
	unsigned int enemyNum = m_Enemies.size();
	for (unsigned int i = 0; i < enemyNum; i++)
	{
		Enemy* enemy = m_Enemies[i].get();

		// �e����
		std::unique_ptr<HomingBullet> bullet = std::make_unique<HomingBullet>();
		bullet->Initialize();
		// ��ɔ���
		bullet->Fire(pos, Vector3::UnitY);
		bullet->SetTarget(enemy);

		m_HomingBullets.push_back(std::move(bullet));
	}
}


