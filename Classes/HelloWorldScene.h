#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"

using keeporder = std::vector<std::pair<cocos2d::Vec2, int>>;

class HelloWorld : public cocos2d::Layer
{
private:
	//モード
	enum Mode
	{
		Line,
		LostHole,
		GaolHole,
		LeftLever,
		RightLever
	};

	//現在のモード
	Mode nowMode;

	//使ったモードを保存する
	std::vector<Mode> modeList;

	//書き込めるPathを保存
	std::string writablePath;

	//modeを変えるためのボタン
	cocos2d::ui::Button* changeButton;

	//mouseを使用するためのリスナー
	cocos2d::EventListenerMouse* mouseListener;

	//key入力を有効にするためのリスナー
	cocos2d::EventListenerKeyboard* keyListener;

	//描画する線のposと先頭か終点かを保存する
	keeporder railDraw;

	//当たり判定の線のデータのposと先頭か終点かを保存する
	keeporder railData;

	//左クリックされて、編集中かどうか
	bool clicked;

	//変数であるrailDrawの配列の長さを保存
	int railDrawSize;
	
	//基本的な図形を描画するための変数
	cocos2d::DrawNode* drawNode;

	//現在のmouseのposを表示するためのLabel
	cocos2d::Label* posLabel;

	//描画する円の大きさ
	cocos2d::Size holeSize;

	//落ちる穴のposを保存するための変数
	std::vector<cocos2d::Vec2> lostHolePos;

	//goal穴のposの変数
	cocos2d::Vec2 goalPos;

	//線を作っている際のクリックした回数
	int clickCount;

	//クリックされた回数を保存
	std::vector<int> clickArray;

	//レバーの円の部分のSprite
	cocos2d::Sprite* leverBackSprite;

	//レバーのレバーの部分のSprite
	cocos2d::Sprite* leverSprite;

	//レバーの位置のY値
	float leverPosY;

	//レバーが影響を与えるエリア位置のY値
	float areaPos;

	//左のレバーのpositionを保存
	std::vector<cocos2d::Vec2> leftLeverPos;

	//左のレバーの影響を与えるエリア位置のpositionを保存
	std::vector<cocos2d::Vec2> leftAreaPos;

	//右のレバーのpositionを保存
	std::vector<cocos2d::Vec2> rightLeverPos;

	//右のレバーの影響を与えるエリア位置のpositionを保存
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