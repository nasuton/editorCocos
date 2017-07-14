#pragma once

#include "cocos2d.h"

class ModalLayer : public cocos2d::Layer
{
public:
	ModalLayer();
	virtual ~ModalLayer();

	virtual bool init();

	CREATE_FUNC(ModalLayer);
private:

};