//=============================================================================
//
// �T�E���h���� [sound.h]
// Author : Akira Tanaka
//
//=============================================================================
#ifndef _SOUND_H_
#define _SOUND_H_

#include "main.h"


class CSound
{
public:
	//�T�E���h�ꗗ
	typedef enum
	{
		SOUND_LABEL_TITLESE_OPEN = 0,
		SOUND_LABEL_TITLESE_LIFT,
		SOUND_LABEL_TITLESE_RIDER,
		SOUND_LABEL_TITLESE_MOVE,
		SOUND_LABEL_TITLESE_ENTER,
		SOUND_LABEL_GAMESE_EXPLOAD,
		SOUND_LABEL_GAMESE_BOMB,
		SOUND_LABEL_GAMESE_ITEM,
		SOUND_LABEL_GAMESE_SHOT,
		SOUND_LABEL_GAMESE_BOSSSHOT,
		SOUND_LABEL_GAMESE_BOSSLOCKON,
		SOUND_LABEL_GAMESE_FALL,
		SOUND_LABEL_GAMESE_SPAWN,
		SOUND_LABEL_GAMESE_TRANSFORM1,
		SOUND_LABEL_GAMESE_TRANSFORM2,
		SOUND_LABEL_GAMESE_TRANSFORM3,
		SOUND_LABEL_GAMESE_BIGBOMB,
		SOUND_LABEL_MAX

	} SOUND_LABEL;
	CSound();								//�R���X�g���N�^
	~CSound();								//�f�X�g���N�^
	HRESULT Init(HWND hWnd);			//������
	void Uninit(void);					//�I��
	HRESULT PlaySound(SOUND_LABEL label);	//�T�E���h�Đ�
	void StopSound(SOUND_LABEL label);		//�T�E���h��~(���x���w��)
	void StopSound(void);					//�T�E���h��~(���ׂ�)

	HRESULT CheckChunk(HANDLE hFile, DWORD format, DWORD* pChunkSize, DWORD* pChunkDataPosition);
	HRESULT ReadChunkData(HANDLE hFile, void* pBuffer, DWORD dwBuffersize, DWORD dwBufferoffset);
private:
	typedef struct
	{
		const char* pFilename;	// �t�@�C����
		int nCntLoop;			// ���[�v�J�E���g
	} SOUNDINFO;
	IXAudio2* m_pXAudio2 = NULL;								// XAudio2�I�u�W�F�N�g�ւ̃C���^�[�t�F�C�X
	IXAudio2MasteringVoice* m_pMasteringVoice = NULL;			// �}�X�^�[�{�C�X
	IXAudio2SourceVoice* m_apSourceVoice[SOUND_LABEL_MAX] = {};	// �\�[�X�{�C�X
	BYTE* m_apDataAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^
	DWORD m_aSizeAudio[SOUND_LABEL_MAX] = {};					// �I�[�f�B�I�f�[�^�T�C�Y

	// �T�E���h�̏��
	SOUNDINFO m_aSoundInfo[SOUND_LABEL_MAX] =
	{
		{"data/SOUND/opener1.wav", 0},	//�J����
		{"data/SOUND/lift2.wav", 0},	//���t�g��������
		{"data/SOUND/rider1.wav", 0},	//�u�[�X�^�[���オ�鉹
		{"data/SOUND/move1.wav", 0},	//���@��������
		{"data/SOUND/Enter1.wav", 0},	//Enter�L�[�������ꂽ��
		{"data/SOUND/expload1.wav", 0},	//�������P
		{"data/SOUND/expload2.wav", 0},	//�������Q
		{"data/SOUND/Item2.wav", 0},	//�A�C�e���l����
		{"data/SOUND/Shot1.wav", 0},	//�ˌ���
		{"data/SOUND/Shot2.wav", 0},	//�{�X�ˌ���
		{"data/SOUND/lockon1.wav", 0},	//�{�X���b�N�I����
		{"data/SOUND/fall.wav", 0},		//�����鉹
		{"data/SOUND/spawn1.wav", 0},	//�Β����o�����鉹
		{"data/SOUND/Metal1.wav", 0},	//�ό`���̌��ʉ�1
		{"data/SOUND/Metal2.wav", 0},	//�ό`���̌��ʉ�2
		{"data/SOUND/Metal3.wav", 0},	//�ό`���̌��ʉ�2
		{"data/SOUND/BigBomb.wav", 0},	//�ό`���̌��ʉ�2


	};
};

#endif
