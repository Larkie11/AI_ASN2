/******************************************************************************/
/*!
\file	Sound.cpp
\author Mok Wei Min
\par	email: 155208U\@mymail.nyp.edu.sg
\brief
Handles music and sound effect for the game using irrklang
*/
/******************************************************************************/
#include "Sound.h"
#include <iostream>

Sound::Sound()
: toplay(true)
{
	musicEngine = irrklang::createIrrKlangDevice();
	isAlreadyPlaying = false;
}
Sound::~Sound()
{
	musicEngine->drop();
}
/******************************************************************************/
/*!
\brief
Plays the background music looped

\param Music
Takes in path of background music to play

\exception None
\return None
*/
/******************************************************************************/
void Sound::playMusic(string Music)
{
	Song = musicEngine->play2D(Music.c_str(), true, false, true);
}
void Sound::setPlayingMenu(bool isplaying)
{
	isAlreadyPlaying = isplaying;
}
bool Sound::isPlayingMenu()
{
	return isAlreadyPlaying;
}
void Sound::onOff(float volume)
{
	if (volume >= 1.f)
		volume = 1.f;
	if (volume <= 0.f)
		volume = 0.f;

	musicEngine->setSoundVolume(volume);
}
float Sound::getOnOff()
{
	return musicEngine->getSoundVolume();
}
/******************************************************************************/
/*!
\brief
Plays the sound effect no loop

\param Music
Takes in path of sound effect to play

\exception None
\return None
*/
/******************************************************************************/
void Sound::playSE(string Music)
{
	SoundEffect = musicEngine->play2D(Music.c_str(), false, false, false);
}
/******************************************************************************/
/*!
\brief
Stop certain music when called

\param Music
Takes in path of background music/sound effect to stop

\exception None
\return None
*/
/******************************************************************************/
void Sound::stopMusic(string Music)
{
	Song->stop();
}