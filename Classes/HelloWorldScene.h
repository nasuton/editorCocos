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
		GaolHole,
		LeftLever,
		RightLever
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

	cocos2d::Sprite* leverBackSprite;

	cocos2d::Sprite* leverSprite;

	float leverPosY;

	float areaPos;

	std::vector<cocos2d::Vec2> leftLeverPos;

	std::vector<cocos2d::Vec2> leftAreaPos;

	std::vector<cocos2d::Vec2> rightLeverPos;

	std::vector<cocos2d::Vec2> rightAreaPos;

public:
	HelloWorld();
	virtual ~HelloWorld();

    static cocos2d::Scene* createScene();

    virtual bool init();

    CREATE_FUNC(HelloWorld);

private:
	//Lineを描画
	void DrawRails();

	//Zキーを押されたとき
	void Cancel(Mode mode_);

	//ModeがLineCreate時に左クリックされた場合
	void LineCreateMouseLeft(float cursorX, float cursorY);

	//ModeがLineCreate時に右クリックされた場合
	void LineCreateMouseRight(float cursorX, float cursorY);

	//holeを作る
	void HoleCreate(float cursorX, float cursorY, cocos2d::Color4F cirlceColor);

	//LeftLeverを作成
	void LeftLeverCreate(float cursorX, float cursorY);

	//LeftLeverを動かす
	void LeftLeverMove(float cursorY);

	//RightLeverを作成
	void RightLeverCreate(float cursorX, float cursorY);

	//RightLeverを動かす
	void RightLeverMove(float cursorY);

	//実際に線を描画する際のcsvを作成
	void CreateRailDrawCSV(std::string filePath, keeporder drawData);

	//描画する線の当たり判定のcsvを作成
	void CreateRailDataCSV(std::string filePath, keeporder physicsData);

	//ステージデータの作成
	void CreateStageJson(std::string filePath);

	//mouseイベント
	void onMouseUp(cocos2d::Event* event);

	void onMouseDown(cocos2d::Event* event);

	void onMouseMove(cocos2d::Event* event);
	
	//キーイベント
	void onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);

	//HelpButtonのイベント
	void HelpButtonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);

	//TypeChangeButtonイベント
	void ChangeButtonEvent(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
};

#endif