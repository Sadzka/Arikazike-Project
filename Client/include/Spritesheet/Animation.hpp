#pragma once
#include <string>
#include <SFML/System/Clock.hpp>

class SpriteSheet;

class Animation{
	friend class SpriteSheet;

	unsigned int            m_frameCurrent;
	unsigned int            m_frameStart;
	unsigned int            m_frameEnd;
	unsigned int            m_frameRow;
	int                     m_frameActionStart;
	int                     m_frameActionEnd;
	float                   m_frameTime;
	float                   m_elapsedTime;
	bool                    m_loop;
	bool                    m_playing;

	std::string             m_name;
	SpriteSheet           * m_spriteSheet;

public:
	Animation();
	~Animation();

	void frameStep();
	void cropSprite();
	void readIn(std::stringstream& l_stream);

	void setSpriteSheet(SpriteSheet* l_sheet);
	int setFrame(unsigned int l_frame);
	void setStartFrame(unsigned int l_frame);
	void setEndFrame(unsigned int l_frame);
	void setFrameRow(unsigned int l_row);
	void setActionStart(unsigned int l_frame);
	void setActionEnd(unsigned int l_frame);
	void setFrameTime(float l_time);
	void setLooping(bool l_loop);
	void setName(const std::string& l_name);

	SpriteSheet * getSpriteSheet();
	unsigned int getFrame();
	unsigned int getStartFrame();
	unsigned int getEndFrame();
	unsigned int getFrameRow();
	int getActionStart();
	int getActionEnd();
	float getFrameTime();
	float getElapsedTime();
	bool isLooping();
	bool isPlaying();
	bool isInAction();
	std::string getName();

	void play();
	void pause();
	void stop();
	void reset();

	void update(const float & dTime);

	friend std::stringstream& operator >>( std::stringstream& l_stream, Animation& a)
	{
		a.readIn(l_stream);
		return l_stream;
	}
};
