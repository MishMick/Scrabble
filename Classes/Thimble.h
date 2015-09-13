#ifndef NBG_GAME_THIMBLE
#define NBG_GAME_THIMBLE

#include "cocos2d.h"
#include <vector>

USING_NS_CC;

class CThimble : public Layer
{
public:
	CThimble();
	virtual ~CThimble();


	void SetId(const int id){m_Id = id;};
	int GetId(){return m_Id;};

	void SetDisabled(const bool disabled){m_Disabled = disabled;};
	bool IsDisabled(){ return m_Disabled;};

	void SetWithBall(const bool withball){m_WithBall= withball;};
	bool IsWithBall(){ return m_WithBall;};

	enum ThimbleState
	{		
		State_No,
		State_Lie,
		State_FlyToField,
		State_StayOnField,
		State_Open,
		State_OpenWithBonus
	};

    static CThimble* create();
    
    virtual bool init();

	Point GetPos(){return m_Pos;};
	void SetPos(Point p)
	{
		m_Pos = p;
	}

	
	

	void ChangeState(const ThimbleState state, const float delay = 0.0f);
	void SetLast(){m_IsLast = true;};
	bool IsActive();
private:	
	virtual bool onTouchBegan(Touch *touch, Event *unused_event);
	virtual void onTouchEnded(Touch *touch, Event *unused_event);
	void SwapLayerWithBall(Node* sender);
	void SwapLayerWithBallEnd(Node* sender);
	void OnAppearEnds(Node* sender);
	void OnCheckLaunchRightThimble(Node* sender);
	Sprite * m_Thimble;
	Sprite * m_Shadow;
	ThimbleState m_State;
	Point m_Pos;

	///Последний ли это элемент в стартовой анимации
	bool m_IsLast;	
	bool m_Disabled;
	bool m_WithBall;
	int m_Id;



};

#define THIMBLE_VECTOR std::vector<CThimble*>

#endif //NBG_GAME_THIMBLE