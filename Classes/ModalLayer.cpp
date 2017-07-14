#include "ModalLayer.h"

USING_NS_CC;

ModalLayer::ModalLayer()
{

}

ModalLayer::~ModalLayer()
{
	
}

bool ModalLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}	

	return true;
}