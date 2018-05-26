#ifndef DEMO_H
#define DEMO_H


#include <SOIL/SOIL.h>
#include <GLM/glm.hpp>
#include <GLM/gtc/matrix_transform.hpp>
#include <GLM/gtc/type_ptr.hpp>
#include <irrKlang/irrKlang.h>
#include <ft2build.h>
#include <freetype/freetype.h>
#include <map>
#include "Game.h"

#define NUM_FRAMES 14
#define FRAME_DUR 120
#define SPEED 0.0015f
#define FONTSIZE 20
#define FONTNAME "fonts/chintzy.ttf"

using namespace glm;
using namespace irrklang;

class Demo :
	public Engine::Game
{ 
	struct Character {
		GLuint TextureID; // ID handle of the glyph texture
		ivec2 Size; // Size of glyph
		ivec2 Bearing; // Offset from baseline to left/top of glyph
		GLuint Advance; // Offset to advance to next glyph
	};
public://Inisialisasi metode dan variable
	Demo();
	~Demo();
	virtual void Init();
	virtual void Update(float deltaTime);
	virtual void Render();
	bool IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2);

private:
	GLuint VBOPlayer, VBOStage, VBOStageFront, VBOFruitMelon1, VBOFruitSemangka1, VBOFruitPisang1, VBOBom1, VBOBom2, VBOTextScore, VBOTitle, VBOPause, VBOGameOver,
		VAOPlayer, VAOStage, VAOStageFront, VAOFruitMelon1, VAOFruitSemangka1, VAOFruitPisang1, VAOBom1, VAOBom2, VAOTextScore, VAOTitle, VAOPause, VAOGameOver,
		EBOPlayer, EBOStage, EBOStageFront, EBOFruitMelon1, EBOFruitSemangka1, EBOFruitPisang1, EBOBom1, EBOBom2,EBOTextScore, EBOTitle, EBOPause, EBOGameOver,
		texturePlayer, textureStage, textureStageFront, textureFruitMelon1, textureFruitSemangka1, textureFruitPisang1, textureBom1, textureBom2, textureTextScore, textureTitle, texturePause, textureGameOver,
		programPlayer, programStage, programStageFront, programFruitMelon1, programFruitSemangka1, programFruitPisang1, programBom1, programBom2, programTextScore, programTitle, programPause, programGameOver;
	float frame_dur = 0, frame_width = 0, frame_height = 0, 
		xpos = 0, ypos = -0.65f, slammer = 1, 
		xpos2 = 0, ypos2 = 0, 
		xpos3 = 0, ypos3 = 0, 
		xposTitle = 3.0f, yposTitle = 3.0f,
		xposPause = 3.0f, yposPause = 3.0f,
		xposGameOver = 3.0f, yposGameOver = 3.0f,
		xposFruitMelon1 = 0.0f, yposFruitMelon1 = 0.5f, 
		xposFruitSemangka1 = -0.5f, yposFruitSemangka1 = 0.5f,
		xposFruitPisang1 = -1.0f, yposFruitPisang1 = 0.5f,
		xposBom1 = 0.5f, yposBom1 = 0.5f,
		xposBom2 = 1.0f, yposBom2 = 0.5f,
		stageWidth, stageHeight, 
		spriteWidth, spriteHeight, 
		floorY=-0.52f, 
		crateWidth=0.0f, crateHeight=0.0f,
		fruitSpeed=0.01f, bombSpeed=0.03f;
	unsigned int frame_idx = 0, flip = 0, counter=0, fruit_frame_idx=0, score=0;
	int anim_state = 3; //1=walk, 2=jump, 3=idle, 4=sleep
	int titleStatus = 1; //1= title, 2=pause, 3=gameover
	void BuildSprite(string name);
	void BuildStage();
	void DrawSprite();
	void UpdateSprite(float deltaTime);
	void MoveSprite(float deltaTime);
	bool pause=true;

	void InitAndPlayMusic();
	void reset();
	void RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color);
	map<GLchar, Character> Characters;
	const char* MP3NAME = "Marshmello - Alone.mp3";
};
#endif

