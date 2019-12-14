#include "Spritesheet/Animation.hpp"
#include "Spritesheet/SpriteSheet.hpp"

Animation::Animation(): m_frameCurrent(0), m_frameStart(0), m_frameEnd(0),
	m_frameRow(0), m_frameTime(0.f), m_elapsedTime(0.f), m_frameActionStart(-1),
	m_frameActionEnd(-1), m_loop(false), m_playing(false){}

Animation::~Animation(){}

void Animation::cropSprite()
{
	m_spriteSheet->cropSprite( sf::IntRect(m_spriteSheet->getSpriteSize().x * m_frameCurrent,
                                       int(m_spriteSheet->getDirection()) * 64 + m_frameRow * 256,
                                           m_spriteSheet->getSpriteSize().x,
                                           m_spriteSheet->getSpriteSize().y) );
}

void Animation::frameStep()
{
    m_frameCurrent++;
    if(m_frameCurrent > m_frameEnd)
    {
        if(m_loop)
            m_frameCurrent = m_frameStart;
        else
            pause();
    }
    setFrame(m_frameCurrent);
}

void Animation::readIn(std::stringstream& l_stream){
	l_stream >> m_frameStart >> m_frameEnd >> m_frameRow
		>> m_frameTime >> m_frameActionStart >> m_frameActionEnd;
}

int Animation::setFrame(unsigned int l_frame){
	if((l_frame >= m_frameStart && l_frame <= m_frameEnd)
    || (l_frame >= m_frameEnd && l_frame <= m_frameStart))
	{
		m_frameCurrent = l_frame;
        return l_frame;
	}
}

bool Animation::isInAction(){
	if (m_frameActionStart == -1 || m_frameActionEnd == -1){ return true; }
	return (m_frameCurrent >= m_frameActionStart && m_frameCurrent <= m_frameActionEnd);
}

void Animation::setSpriteSheet(SpriteSheet* l_sheet){ m_spriteSheet = l_sheet; }
void Animation::setStartFrame(unsigned int l_frame){ m_frameStart = l_frame; }
void Animation::setEndFrame(unsigned int l_frame){ m_frameEnd = l_frame; }
void Animation::setFrameRow(unsigned int l_row){ m_frameRow = l_row; }
void Animation::setActionStart(unsigned int l_frame){ m_frameActionStart = l_frame; }
void Animation::setActionEnd(unsigned int l_frame){ m_frameActionEnd = l_frame; }
void Animation::setFrameTime(float l_time){ m_frameTime = l_time; }
void Animation::setLooping(bool l_loop){ m_loop = l_loop; }
void Animation::setName(const std::string& l_name){ m_name = l_name; }

SpriteSheet* Animation::getSpriteSheet(){ return m_spriteSheet; }
unsigned int Animation::getFrame(){ return m_frameCurrent; }
unsigned int Animation::getStartFrame(){ return m_frameStart; }
unsigned int Animation::getEndFrame(){ return m_frameEnd; }
unsigned int Animation::getFrameRow(){ return m_frameRow; }
int Animation::getActionStart(){ return m_frameActionStart; }
int Animation::getActionEnd(){ return m_frameActionEnd; }
float Animation::getFrameTime(){ return m_frameTime; }
float Animation::getElapsedTime(){ return m_elapsedTime; }
std::string Animation::getName(){ return m_name; }
bool Animation::isLooping(){ return m_loop; }
bool Animation::isPlaying(){ return m_playing; }

void Animation::play(){ m_playing = true; }
void Animation::pause(){ m_playing = false; }
void Animation::stop(){ m_playing = false; reset(); }
void Animation::reset()
{
	m_frameCurrent = m_frameStart;
	m_elapsedTime = 0.0f;
	cropSprite();
}

void Animation::update(const float & dTime)
{

	if (!m_playing)
        return;

	m_elapsedTime += dTime;

	if (m_elapsedTime < m_frameTime)
        return;

	frameStep();
	cropSprite();
	m_elapsedTime = 0;
}


