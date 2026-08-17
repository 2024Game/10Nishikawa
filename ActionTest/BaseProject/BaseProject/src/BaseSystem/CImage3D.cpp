#include "CImage3D.h"

// 3D��Ԃł̏k�ځi3D��Ԃł�1�̃T�C�Y�ɑ�������s�N�Z�����j
#define WORLD_UNIT_PER_PIXEL 256.0f

//�R���X�g���N�^
CImage3D::CImage3D(std::string path,
	ETag tag,
	ETaskPriority prio, int sortOrder,
	ETaskPauseType pause,
	bool dontDelete, bool addTaskList)
	: CObjectBase(tag, prio, sortOrder, pause, dontDelete, addTaskList)
	, mAlignment(EAlignment::eDefault)
	, mOffsetPos(0.0f, 0.0f)
	, mUV(0.0f, 0.0f, 1.0f, 1.0f)
	, mWolrdUnitPerPixel(WORLD_UNIT_PER_PIXEL)
	, mpAnimData(nullptr)
	, mAnimNo(0)
	, mElapsedTime(0.0f)
	, mIsBillboard(false)
	, mIsDepthTest(true)
	, mIsDepthMask(false)
	, mIsLighting(false)
{
	// �e�N�X�`���ǂݍ���
	Load(path);

	SetColor(CColor::white);

	// UV�ݒ�
	SetUV(CRect(0.0f, 0.0f, 1.0f, 1.0f));
}

//�f�X�g���N�^
CImage3D::~CImage3D()
{
}

// �e�N�X�`���ǂݍ���
void CImage3D::Load(std::string path)
{
	// �e�N�X�`���ǂݍ���
	mMaterial.LoadTexture(path, path, false);
	CTexture* tex = mMaterial.Texture();
	if (tex != nullptr)
	{
		// �ǂݍ��񂾃e�N�X�`����
		CVector2 size;
		size.X(tex->Header().width);
		size.Y(tex->Header().height);
		SetSize(size);
	}
}

// �C���[�W�̃A���C�����g���擾
EAlignment CImage3D::GetAlignment() const
{
	return mAlignment;
}

// �C���[�W�̃A���C�����g��ݒ�
void CImage3D::SetAlignment(EAlignment align)
{
	mAlignment = align;
	ApplyVertices();
}

// 2D��Ԃł̃I�t�Z�b�g���W���擾
const CVector2& CImage3D::GetOffsetPos() const
{
	return mOffsetPos;
}

// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�
void CImage3D::SetOffsetPos(const float& x, const float& y)
{
	SetOffsetPos(CVector2(x, y));
}

// 2D��Ԃł̃I�t�Z�b�g���W��ݒ�iCVector2�Łj
void CImage3D::SetOffsetPos(const CVector2& pos)
{
	mOffsetPos = pos;
}

// �T�C�Y�擾
const CVector2& CImage3D::GetSize() const
{
	return mSize;
}

// �T�C�Y�ݒ�
void CImage3D::SetSize(const float& x, const float& y)
{
	SetSize(CVector2(x, y));
}

// �T�C�Y�ݒ�iCVector2�Łj
void CImage3D::SetSize(const CVector2& size)
{
	mSize = size;
	ApplyVertices();
}

// UV�擾
const CRect& CImage3D::GetUV() const
{
	return mUV;
}

// UV�ݒ�
void CImage3D::SetUV(const float& left, const float& top, const float& right, const float& bottom)
{
	CRect rect;
	rect.SetPos(left, top);
	rect.W(right - left);
	rect.H(bottom - top);
	SetUV(rect);
}

// UV�ݒ�iCRect�Łj
void CImage3D::SetUV(const CRect& uv)
{
	mUV = uv;

	float l = uv.X();
	float t = 1.0f - uv.Y();
	float r = l + uv.W();
	float b = t - uv.H();

	mT[0].UV
	(
		CVector(r, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, b, 0.0f)
	);
	mT[1].UV
	(
		CVector(l, t, 0.0f),
		CVector(l, b, 0.0f),
		CVector(r, t, 0.0f)
	);
}

// 3D��Ԃł̏k�ڂ�ݒ�
void CImage3D::SetWorldUnitPerPixel(float pixel)
{
	mWolrdUnitPerPixel = pixel;
	SetSize(mSize);
}

// �A�j���[�V�����f�[�^��ݒ�
void CImage3D::SetAnimData(TexAnimData* animData)
{
	CTexture* tex = mMaterial.Texture();
	if (tex == nullptr) return;

	mpAnimData = animData;
	mAnimNo = 0;
	mElapsedTime = 0.0f;

	// �A�j���[�V�������L���ł���΁A
	// ��ԍŏ��̉摜��UV��ݒ�
	if (IsEnableAnim())
	{
		CRect uv = mMaterial.Texture()->CalcUV
		(
			mpAnimData->row,
			mpAnimData->col,
			0
		);
		SetUV(uv);

		CVector2 size;
		float ratio = (float)mpAnimData->col / mpAnimData->row;
		size.X(tex->Header().width);
		size.Y(tex->Header().width * ratio);
		SetSize(size);
	}
}

// �A�j���[�V�������I��������ǂ���
bool CImage3D::IsEndAnim()
{
	if (mpAnimData == nullptr) return true;
	int size = mpAnimData->frames.size();
	return mAnimNo == size;
}

// �r���{�[�h�̃I���I�t��ݒ�
void CImage3D::SetBillboard(bool enable)
{
	mIsBillboard = enable;
}

// �f�v�X�e�X�g�̃I���I�t��ݒ�
void CImage3D::SetDepthTest(bool enable)
{
	mIsDepthTest = enable;
}

// �f�v�X�e�X�g�̃I���I�t��ݒ�
void CImage3D::SetDepthMask(bool enable)
{
	mIsDepthMask = enable;
}

// ���C�e�B���O�̃I���I�t��ݒ�
void CImage3D::SetLighting(bool enable)
{
	mIsLighting = enable;
}

// �ύX�𒸓_�ɔ��f
void CImage3D::ApplyVertices()
{
	CVector s = mSize * (1.0f / mWolrdUnitPerPixel) * 2.0f;
	CVector hs = s * 0.5f;

	CVector start = CVector::zero;

	if (mAlignment == EAlignment::eUpperLeft)			start = CVector(0.0f,		0.0f,		0.0f);
	else if (mAlignment == EAlignment::eUpperCenter)	start = CVector(-hs.X(),	0.0f,		0.0f);
	else if (mAlignment == EAlignment::eUpperRight)		start = CVector(-s.X(),		0.0f,		0.0f);

	else if (mAlignment == EAlignment::eMiddleLeft)		start = CVector(0.0f,		-hs.Y(),	0.0f);
	else if (mAlignment == EAlignment::eMiddleCenter)	start = CVector(-hs.X(),	-hs.Y(),	0.0f);
	else if (mAlignment == EAlignment::eMiddleRight)	start = CVector(-s.X(),		-hs.Y(),	0.0f);

	else if (mAlignment == EAlignment::eLowerLeft)		start = CVector(0.0f,		-s.Y(),		0.0f);
	else if (mAlignment == EAlignment::eLowerCenter)	start = CVector(-hs.X(),	-s.Y(),		0.0f);
	else if (mAlignment == EAlignment::eLowerRight)		start = CVector(-s.X(),		-s.Y(),		0.0f);

	// �O�p�`�̒��_���W�ݒ�
	mT[0].Vertex
	(
		start + CVector(s.X(), s.Y(), 0.0f),
		start + CVector(0.0f, 0.0f, 0.0f),
		start + CVector(s.X(), 0.0f, 0.0f)
	);
	mT[1].Vertex
	(
		start + CVector(0.0f, s.Y(), 0.0f),
		start + CVector(0.0f, 0.0f, 0.0f),
		start + CVector(s.X(), s.Y(), 0.0f)
	);

	// �@����Z������
	mT[0].Normal(CVector(0.0f, 0.0f, 1.0f));
	mT[1].Normal(CVector(0.0f, 0.0f, 1.0f));
}

// �A�j���[�V�������Đ��ł��邩�ǂ���
bool CImage3D::IsEnableAnim() const
{
	if (mpAnimData == nullptr) return false;
	if (mpAnimData->frames.size() == 0) return false;
	return true;
}

// �X�V
void CImage3D::Update()
{
	// �A�j���[�V�������L���ł����
	if (IsEnableAnim())
	{
		int size = mpAnimData->frames.size();
		if (mAnimNo < size)
		{
			TexAnimFrameData data = mpAnimData->frames[mAnimNo];
			// ���݂̉摜�̕\�����Ԃ𒴂�����
			if (mElapsedTime >= data.time)
			{
				// ���̉摜�֐؂�ւ�
				mElapsedTime -= data.time;

				mAnimNo++;
				if (mAnimNo >= size)
				{
					if (mpAnimData->loop)
					{
						mAnimNo -= size;
					}
					else
					{
						mAnimNo = size;
					}
				}
				CRect uv = mMaterial.Texture()->CalcUV
				(
					mpAnimData->row,
					mpAnimData->col,
					mAnimNo
				);
				SetUV(uv);
			}
			// �o�ߎ��Ԃ��v��
			mElapsedTime += Times::DeltaTime();
		}

	}
}

// �`��
void CImage3D::Render()
{
	Render(&mMaterial);
}

// �`��i�}�e���A���w��Łj
void CImage3D::Render(CMaterial* mpMaterial)
{
	CTexture* tex = mMaterial.Texture();
	if (tex == nullptr) return;

	// �s��̕ۑ�
	glPushMatrix();

	// ���g�̍s��
	CMatrix m;

	// �r���{�[�h���L���Ȃ��
	if (mIsBillboard)
	{
		// ��ɃJ�����̕�������������
		CCamera* cam = CCamera::CurrentCamera();
		CMatrix camMtx = cam->GetViewMatrix().Inverse();
		camMtx.Position(CVector::zero);
		CMatrix s, r, t;
		s.Scale(mScale);
		r = mRotation.Matrix();
		t.Translate(mPosition);
		m = s * r * camMtx * t;
	}
	// �r���{�[�h�������Ȃ��
	else
	{
		// �ʏ�̍s����擾
		m = Matrix();
	}

	// �s��𔽉f
	glMultMatrixf(m.M());

	// �I�t�Z�b�g���W�𔽉f
	CVector2 offset = mOffsetPos * (1.0f / mWolrdUnitPerPixel);
	glTranslatef(offset.X(), offset.Y(), 0.0f);

	// �e�ݒ�̃t���O�̏�Ԃɍ��킹�āA�I�t�ɂ���
	if (!mIsDepthTest) glDisable(GL_DEPTH_TEST);	// �f�v�X�e�X�g
	if (!mIsDepthMask) glDepthMask(false);			// �f�v�X��������
	if (!mIsLighting) glDisable(GL_LIGHTING);		// ���C�e�B���O

	// �}�e���A���K�p
	mpMaterial->Enabled(mColor);
	// �O�p�`�̕`��
	mT[0].Render();
	mT[1].Render();
	// �}�e���A������
	mpMaterial->Disabled();

	// �e�ݒ�����ɖ߂�
	glEnable(GL_DEPTH_TEST);	// �f�v�X�e�X�g
	glDepthMask(true);			// �f�v�X��������
	glEnable(GL_LIGHTING);		// ���C�e�B���O

	// �s���߂�
	glPopMatrix();
}
