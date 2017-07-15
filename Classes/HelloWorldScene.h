#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using keeporder = std::vector<std::pair<cocos2d::Vec2, int>>;

class HelloWorld : public cocos2d::Layer
{
private:
	enum Mode
	{
		Line,
		LostHole,
		GaolHole
	};

	Mode nowMode;

	std::vector<Mode> modeList;

	std::string writablePath;

	cocos2d::ui::Button* changeButton;

	cocos2d::EventListenerMouse* mouseListener;

	cocos2d::EventListenerKeyboard* keyListener;

	keeporder railDraw;

	keeporder railData;

	bool clicked;

	int railDrawSize;

	cocos2d::DrawNode* drawNode;

	cocos2d::Label* posLabel;

	cocos2d::Size holeSize;

	std::vector<cocos2d::Vec2> lostHolePos;

	cocos2d::Vec2 goalPos;

	int clickCount;

	std::vector<int> clickArray;

public:
	HelloWorld();
	virtual ~HelloWorld();

    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

private:
	//Line��`��
	void DrawRails();

	//Z�L�[�������ꂽ�Ƃ�
	void Cancel(Mode mode_);

	//Mode��LineCreate���ɍ��N���b�N���ꂽ�ꍇ
	void LineCreateMouseLeft(cocos2d::Vec2 pos);

	//Mode��LineCreate���ɉE�N���b�N���ꂽ�ꍇ
	void LineCreateMouseRight(cocos2d::Vec2 pos);

	//hole�����
	void HoleCreate(cocos2d::Vec2 pos, cocos2d::Color4F cirlceColor);

	//���ۂɐ���`�悷��ۂ�csv���쐬
	void CreateRailDrawCSV(std::string filePath, keeporder drawData);

	//�`�悷����̓����蔻���csv���쐬
	void CreateRailDataCSV(std::string filePath, keeporder physicsData);

	//�X�e�[�W�f�[�^�̍쐬
	void CreateStageJson(std::string filePath);

	//mouse�C�x���g
	void onMouseUp(cocos2d::Event* event);

	void onMouseDown(cocos2d::Event* event);

	void onMouseMove(cocos2d::Event* event);
	
	//�L�[�C�x���g
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	//HelpButton�̃C�x���g
	void HelpButtonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//TypeChangeButton�C�x���g
	void ChangeButtonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif