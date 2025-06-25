#ifndef CXPLAYER_H
#define CXPLAYER_H

#include "CXCharacter.h"
#include "CInput.h"

class CXPlayer : public CXCharacter
{
public:
	// XVˆ—
	void Update();

private:
	CInput mInput;
};
#endif // !CXPLAYER_H