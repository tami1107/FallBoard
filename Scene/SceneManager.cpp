#include "SceneManager.h"
#include <cassert>

#include "SceneTitle.h"
#include "Pad.h"


SceneManager::SceneManager()
{
	m_pScene = nullptr;
}
SceneManager::~SceneManager()
{
}

void SceneManager::Init()
{
	m_pScene = new SceneTitle;

	m_pScene->Init();
}

void SceneManager::End()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	m_pScene->End();


	delete m_pScene;
}

void SceneManager::Update()
{
	assert(m_pScene);
	if (!m_pScene)	return;

	// �p�b�h�̃A�b�v�f�[�g����
	Pad::Update();

	SceneBase* pScene = m_pScene->Update();
	if (pScene != m_pScene)
	{
		// �O�̃V�[���̏I������
		m_pScene->End();

		delete m_pScene;

		m_pScene = pScene;

		m_pScene->Init();
	}
}

void SceneManager::Draw()
{
	assert(m_pScene);
	if (!m_pScene)	return;
	m_pScene->Draw();
}