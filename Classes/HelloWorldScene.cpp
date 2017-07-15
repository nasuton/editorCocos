#include "HelloWorldScene.h"
#include <iostream>
#include <fstream>
#include "json/document.h"
#include "json/rapidjson.h"
#include "json/filewritestream.h"
#include "json/prettywriter.h"

USING_NS_CC;

HelloWorld::HelloWorld()
	:clicked(false)
	, railDrawSize(0)
	, holeSize(30.0f, 30.0f)
	,goalPos(0.0f, 0.0f)
	,clickCount(0)
{
	mouseListener = EventListenerMouse::create();
	mouseListener->onMouseUp = CC_CALLBACK_1(HelloWorld::onMouseUp, this);
	mouseListener->onMouseDown = CC_CALLBACK_1(HelloWorld::onMouseDown, this);
	mouseListener->onMouseMove = CC_CALLBACK_1(HelloWorld::onMouseMove, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(mouseListener, this);

	keyListener = EventListenerKeyboard::create();
	keyListener->onKeyPressed = CC_CALLBACK_2(HelloWorld::onKeyPressed, this);
	Director::getInstance()->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyListener, this);
}

HelloWorld::~HelloWorld()
{
	Director::getInstance()->getEventDispatcher()->removeEventListener(mouseListener);
	Director::getInstance()->getEventDispatcher()->removeEventListener(keyListener);
}

Scene* HelloWorld::createScene()
{
	auto scene = Scene::create();

	scene->addChild(HelloWorld::create());

	return scene;
}

bool HelloWorld::init()
{
	if (!Layer::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();

	nowMode = Mode::Line;

	writablePath = FileUtils::getInstance()->getWritablePath();
	log(u8"pathは%s", writablePath.c_str());

	auto helpButton = ui::Button::create("helpButton.png");
	helpButton->setTitleFontName("fonts/arial.ttf");
	helpButton->setTitleColor(Color3B::WHITE);
	helpButton->setTitleText("Help");
	helpButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::HelpButtonEvent, this));
	helpButton->setPosition(Vec2(helpButton->getContentSize().width * 0.5f, visibleSize.height - (helpButton->getContentSize().height * 0.5f)));
	this->addChild(helpButton);

	changeButton = ui::Button::create("changeButton.png");
	changeButton->setTitleFontName("fonts/arial.ttf");
	changeButton->setTitleColor(Color3B::WHITE);
	changeButton->setTitleText("LineCreate");
	changeButton->addTouchEventListener(CC_CALLBACK_2(HelloWorld::ChangeButtonEvent, this));
	changeButton->setPosition(Vec2(visibleSize.width - (changeButton->getContentSize().width * 0.5f), visibleSize.height - (changeButton->getContentSize().height * 0.5f)));
	this->addChild(changeButton);

	posLabel = Label::create("posX : 0, posY : 0", "fonts/arial.ttf", 15.0f);
	posLabel->setTextColor(Color4B::WHITE);
	posLabel->setPosition(Vec2(visibleSize.width * 0.7f, 15.0f));
	posLabel->setHorizontalAlignment(TextHAlignment::CENTER);
	this->addChild(posLabel);

	railData =
	{
		std::make_pair(Vec2(60, 300), 0),
		std::make_pair(Vec2(60, 60), 0),
		std::make_pair(Vec2(300, 60), 0),
		std::make_pair(Vec2(300, 600), 0),
		std::make_pair(Vec2(60, 600), 1)
	};

	railDraw =
	{
		std::make_pair(Vec2(60, 600), 0),
		std::make_pair(Vec2(60, 60), 1),
		std::make_pair(Vec2(60, 60), 0),
		std::make_pair(Vec2(300, 60), 1),
		std::make_pair(Vec2(300, 60), 0),
		std::make_pair(Vec2(300, 600), 1),
		std::make_pair(Vec2(60, 600), 0),
		std::make_pair(Vec2(300, 600), 1),
	};

	for (int i = 0; i < railDraw.size(); i++)
	{
		if (railDraw[i].second == 1)
		{
			DrawNode* rail = DrawNode::create();
			rail->drawSegment(railDraw[i - 1].first, railDraw[i].first, 2.0f, Color4F::WHITE);
			this->addChild(rail);
		}
	}

	railDrawSize = (int)railDraw.size();

	drawNode = DrawNode::create();
	this->addChild(drawNode);

	return true;
}

void HelloWorld::DrawRails()
{
	DrawNode* rails = DrawNode::create();
	rails->drawSegment(railDraw[railDrawSize - 2].first, railDraw[railDrawSize - 1].first, 2.0f, Color4F::WHITE);
	rails->setName(StringUtils::format("rails%i", railDrawSize));
	this->addChild(rails);
}

void HelloWorld::Cancel(Mode mode_)
{
	switch (mode_)
	{
	case Mode::Line:
		for (int i = 0; i < clickArray.back(); i++)
		{
			railData.pop_back();
		}

		for (int i = 0; i < clickArray.back() - 1; i++)
		{
			this->removeChildByName(StringUtils::format("rails%i", (int)railDraw.size()));
			railDraw.pop_back();
			railDraw.pop_back();
		}

		clickArray.pop_back();

		railDrawSize = (int)railDraw.size();
		break;
	case Mode::LostHole:
		this->removeChildByName(StringUtils::format("lostHole%i", (int)lostHolePos.size()));
		lostHolePos.pop_back();
		break;
	case Mode::GaolHole:
		this->removeChildByName("gaolHole");
		break;
	default:
		break;
	}

	modeList.pop_back();

}

void HelloWorld::LineCreateMouseLeft(cocos2d::Vec2 pos)
{
	if (!clicked)
	{
		railDraw.push_back(std::make_pair(Vec2(pos), 0));
		railData.push_back(std::make_pair(Vec2(pos), 0));
		railDrawSize = (int)railDraw.size();
		clickCount += 1;
		modeList.push_back(nowMode);
		clicked = true;
	}
	else
	{
		railDraw.push_back(std::make_pair(Vec2(pos), 1));
		railData.push_back(std::make_pair(Vec2(pos), 1));
		railDrawSize = (int)railDraw.size();
		this->DrawRails();
		drawNode->clear();
		clickCount += 1;
		clickArray.push_back(clickCount);
		clickCount = 0;
		clicked = false;
	}
}

void HelloWorld::LineCreateMouseRight(cocos2d::Vec2 pos)
{
	if (!clicked)
	{
		return;
	}

	railDraw.push_back(std::make_pair(Vec2(pos), 1));
	railDrawSize = (int)railDraw.size();
	this->DrawRails();
	drawNode->clear();
	railDraw.push_back(std::make_pair(Vec2(pos), 0));
	railData.push_back(std::make_pair(Vec2(pos), 0));
	clickCount += 1;
}

void HelloWorld::HoleCreate(cocos2d::Vec2 pos, cocos2d::Color4F cirlceColor)
{
	auto holeCirlce = DrawNode::create();
	holeCirlce->drawDot(pos, holeSize.width * 0.5f, cirlceColor);
	this->addChild(holeCirlce);

	pos.x = floorf(pos.x) - (holeSize.width * 0.5f);
	pos.y = floorf(pos.y) - (holeSize.height * 0.5f);

	switch (nowMode)
	{
	case Mode::LostHole:
		lostHolePos.push_back(pos);
		holeCirlce->setName(StringUtils::format("lostHole%i", (int)lostHolePos.size()));
		break;
	case Mode::GaolHole:
		if (this->getChildByName("gaolHole"))
		{
			this->removeChildByName("gaolHole");
		}
		holeCirlce->setName("gaolHole");
		goalPos = pos;
		break;
	default:
		break;
	}
}

void HelloWorld::CreateRailDrawCSV(std::string filePath, keeporder drawData)
{
	std::ofstream ofs(filePath + "raildraw.csv");
	for (int i = 0; i < drawData.size(); i++)
	{
		ofs << StringUtils::format("%i, %i, %i", (int)drawData[i].first.x, (int)drawData[i].first.y, drawData[i].second) << std::endl;
	}
}

void HelloWorld::CreateRailDataCSV(std::string filePath, keeporder physicsData)
{
	std::ofstream ofs(filePath + "raildata.csv");
	for (int i = 0; i < physicsData.size(); i++)
	{
		ofs << StringUtils::format("%i, %i, %i", (int)physicsData[i].first.x, (int)physicsData[i].first.y, physicsData[i].second) << std::endl;
	}
}

void HelloWorld::CreateStageJson(std::string filePath)
{
	//jsonデータの作成
	rapidjson::Document document;
	document.SetObject();
	rapidjson::Document::AllocatorType& allocator = document.GetAllocator();
	rapidjson::Value jsonArray(rapidjson::kArrayType);
	// 要素生成
	for (int i = 0; i < lostHolePos.size(); i++) {
		rapidjson::Value pos(rapidjson::kObjectType);
		pos.AddMember("x", lostHolePos[i].x, allocator);
		pos.AddMember("y", lostHolePos[i].y, allocator);
		rapidjson::Value size(rapidjson::kObjectType);
		size.AddMember("width", holeSize.width, allocator);
		size.AddMember("height", holeSize.height, allocator);
		rapidjson::Value block(rapidjson::kObjectType);
		block.AddMember("position", pos, allocator);
		block.AddMember("size", size, allocator);
		//配列に追加
		jsonArray.PushBack(block, allocator);
	}
	// JSON配列追加
	document.AddMember("lostHole", jsonArray, allocator);

	//ゴール作成
	rapidjson::Value goalHolePos(rapidjson::kObjectType);
	goalHolePos.AddMember("x", goalPos.x, allocator);
	goalHolePos.AddMember("y", goalPos.y, allocator);

	rapidjson::Value goalHoleSize(rapidjson::kObjectType);
	goalHoleSize.AddMember("width", holeSize.width, allocator);
	goalHoleSize.AddMember("height", holeSize.height, allocator);

	rapidjson::Value goalHole(rapidjson::kObjectType);
	goalHole.AddMember("position", goalHolePos, allocator);
	goalHole.AddMember("size", goalHoleSize, allocator);

	document.AddMember("goalHole", goalHole, allocator);

	std::string jsonPath = filePath + "tenYenStage.json";

	FILE* fp = fopen(jsonPath.c_str(), "wb");

	char writeBuffer[4096];

	rapidjson::FileWriteStream os(fp, writeBuffer, sizeof(writeBuffer));
	rapidjson::PrettyWriter<rapidjson::FileWriteStream> writer(os);
	document.Accept(writer);
	fclose(fp);
}

void HelloWorld::onMouseUp(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;
}

void HelloWorld::onMouseDown(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;

	if (50.0f >= e->getCursorX() || e->getCursorX() >= 310.0f)
	{
		return;
	}

	if (50.0f >= e->getCursorY() || e->getCursorY() >= 610.0f)
	{
		return;
	}

	Vec2 minMaxPos = Vec2(0.0f, 0.0f);

	if (60.0f >= e->getCursorX())
	{
		minMaxPos.x = 60.0f;
	}
	else if (e->getCursorX() >= 300.0f)
	{
		minMaxPos.x = 300.0f;
	}
	else
	{
		minMaxPos.x = e->getCursorX();
	}

	if (60.0f >= e->getCursorY())
	{
		minMaxPos.y = 60.0f;
	}
	else if (e->getCursorY() >= 600.0f)
	{
		minMaxPos.y = 600.0f;
	}
	else
	{
		minMaxPos.y = e->getCursorY();
	}

	if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_LEFT)
	{
		switch (nowMode)
		{
		case Mode::Line:
			this->LineCreateMouseLeft(minMaxPos);
			break;

		case Mode::LostHole:
			this->HoleCreate(minMaxPos, Color4F::WHITE);
			modeList.push_back(nowMode);
			break;

		case Mode::GaolHole:
			this->HoleCreate(minMaxPos, Color4F::RED);
			modeList.push_back(nowMode);
			break;

		default:
			break;
		}
	}
	else if (e->getMouseButton() == EventMouse::MouseButton::BUTTON_RIGHT)
	{
		this->LineCreateMouseRight(minMaxPos);
	}
}

void HelloWorld::onMouseMove(cocos2d::Event* event)
{
	EventMouse* e = (EventMouse*)event;

	switch (nowMode)
	{
	case Mode::Line:
		if (clicked)
		{
			drawNode->clear();
			drawNode->drawSegment(railDraw[railDrawSize - 1].first, Vec2(e->getCursorX(), e->getCursorY()), 2.0f, Color4F::WHITE);
		}
		break;

	case Mode::LostHole:
		drawNode->clear();
		drawNode->drawCircle(Vec2(e->getCursorX(), e->getCursorY()), holeSize.width * 0.5f, 0.0f, 360, false, 1, 1, Color4F::WHITE);
		break;

	case Mode::GaolHole:
		drawNode->clear();
		drawNode->drawCircle(Vec2(e->getCursorX(), e->getCursorY()), holeSize.width * 0.5f, 0.0f, 360, false, 1, 1, Color4F::RED);
		break;

	default:
		break;
	}

	posLabel->setString("posX : " + StringUtils::toString((int)e->getCursorX()) + ", posY : " + StringUtils::toString((int)e->getCursorY()));
}

void HelloWorld::onKeyPressed(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event)
{
	switch (keyCode)
	{
	case EventKeyboard::KeyCode::KEY_S:
		if (!clicked)
		{
			this->CreateRailDrawCSV(writablePath, railDraw);
			this->CreateRailDataCSV(writablePath, railData);
			if (lostHolePos.size() != 0)
			{
				this->CreateStageJson(writablePath);
			}			
			log(u8"Saveしました");
		}
		else
		{
			log(u8"編集中なのでSaveできません");
		}

		break;
	case EventKeyboard::KeyCode::KEY_Z:
		if (modeList.size() != 0)
		{
			this->Cancel(modeList.back());
		}		

		break;
	default:
		break;
	}
}

void HelloWorld::HelpButtonEvent(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	
}

void HelloWorld::ChangeButtonEvent(cocos2d::Ref * sender, cocos2d::ui::Widget::TouchEventType type)
{
	if (clicked)
	{
		return;
	}

	if (type == ui::Widget::TouchEventType::ENDED)
	{
		switch (nowMode)
		{
		case Mode::Line:
			changeButton->setTitleText("LostHoleCreate");
			nowMode = Mode::LostHole;
			break;

		case Mode::LostHole:
			changeButton->setTitleText("GoalHoleCreate");
			nowMode = Mode::GaolHole;
			break;

		case Mode::GaolHole:
			changeButton->setTitleText("LineCreate");
			nowMode = Mode::Line;
			drawNode->clear();
			break;
		default:
			break;
		}
	}
}
