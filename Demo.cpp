#include "Demo.h"

Demo::Demo()
{
}

Demo::~Demo()
{
}

void Demo::Init()
{
	BuildSprite("Front");
	BuildSprite("Player");
	BuildSprite("Stage");
	BuildSprite("FruitMelon1");
	BuildSprite("FruitSemangka1");
	BuildSprite("FruitPisang1");
	BuildSprite("Bom1");
	BuildSprite("Bom2");
	BuildSprite("TextScore");
	BuildSprite("Title");
	BuildSprite("GameOver");
	BuildSprite("Pause");
	InitAndPlayMusic();
}

void Demo::Update(float deltaTime)
{
	UpdateSprite(deltaTime);
	MoveSprite(deltaTime);
}

void Demo::Render()
{
	//Setting Viewport
	glViewport(0, 0, screenWidth, screenHeight);

	//Clear the color and depth buffer
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//Set the background color
	glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

	DrawSprite();
}

void Demo::UpdateSprite(float deltaTime)
{
	if (pause == false) {
		// Update animation
		frame_dur += deltaTime;
		//Animasi berjalan
		if (anim_state == 1 && frame_dur > FRAME_DUR) {
			frame_dur = 0;
			if (!flip) { //Akan menampilkan sprite dari sprite sheet, saat diflip urutan sprite sudah disesuaikan.
				if (frame_idx > 1) frame_idx = 0;  else frame_idx++;
			}
			else {
				if (frame_idx < NUM_FRAMES - 2) frame_idx = NUM_FRAMES - 1;  else frame_idx--;
			}
		}
		//Melompat
		if (anim_state == 2 && frame_dur > FRAME_DUR) {
			frame_dur = 0;
			if (!flip) {//Akan menampilkan sprite dari sprite sheet, saat diflip urutan sprite sudah disesuaikan.
				if (frame_idx != 10) { //Menampilkan animasi lompat. Tapi berhenti di akhir animasi. Menampilkan sprite lompat.
					if (frame_idx < 8 || frame_idx > 10) frame_idx = 8;  else frame_idx++;
				}
			}
			else {
				if (frame_idx != 4) {
					if (frame_idx >= 6 || frame_idx < 4) frame_idx = 5;  else frame_idx--;
				}
			}
		}
		//Animasi ketika tidak bergerak. Tiga sprite di loop, termasuk flipnya.
		if (anim_state == 3 && frame_dur > FRAME_DUR) {
			frame_dur = 0;
			if (!flip) {
				if (frame_idx == 13 || frame_idx < 11) frame_idx = 11;  else frame_idx++;
			}
			else {
				if (frame_idx == 0 || frame_idx > 3) frame_idx = 2;  else frame_idx--;
			}
		}
		//Animasi sleep. Animasi berhenti ketika megaman sudah tidur di lantai. 
		if (anim_state == 4 && frame_dur > FRAME_DUR) {
			frame_dur = 0;
			if (!flip) {
				if (frame_idx != 7) {
					if (frame_idx < 3 || frame_idx >= 8) frame_idx = 3;  else frame_idx++;
				}
				else {
					frame_idx = 7;
				}
			}
			else {
				if (frame_idx != 6) {
					if (frame_idx < 6 || frame_idx >= 11) frame_idx = 10;  else frame_idx--;
				}
				else {
					frame_idx = 6;
				}
			}
		}
	}
	// Pass frameIndex to shader
	GLint location = glGetUniformLocation(this->programPlayer, "frameIndex");
	UseShader(this->programPlayer);
	glUniform1i(location, frame_idx);

	// Pass frameIndex to shader
	GLint locationFruitMelon1 = glGetUniformLocation(this->programFruitMelon1, "frameIndex");
	UseShader(this->programFruitMelon1);
	glUniform1i(locationFruitMelon1,0);

	// Pass frameIndex to shader
	GLint locationFruitSemangka1 = glGetUniformLocation(this->programFruitSemangka1, "frameIndex");
	UseShader(this->programFruitSemangka1);
	glUniform1i(locationFruitSemangka1, 1);

	// Pass frameIndex to shader
	GLint locationFruitPisang1 = glGetUniformLocation(this->programFruitPisang1, "frameIndex");
	UseShader(this->programFruitPisang1);
	glUniform1i(locationFruitPisang1, 2);

	// Pass frameIndex to shader
	GLint locationBom1 = glGetUniformLocation(this->programBom1, "frameIndex");
	UseShader(this->programBom1);
	glUniform1i(locationBom1, 3);

	// Pass frameIndex to shader
	GLint locationBom2 = glGetUniformLocation(this->programBom2, "frameIndex");
	UseShader(this->programBom2);
	glUniform1i(locationBom2, 4);
}

void Demo::MoveSprite(float deltaTime)
{
	if (pause == true) {
		if (IsKeyDown(SDLK_SPACE) || IsKeyDown(SDL_CONTROLLER_BUTTON_START) ){
			if (titleStatus == 3) {
				reset();
			}
			pause = false;
			titleStatus = 2;
			SDL_Delay(200);
		}
	}
	else{
		if (IsKeyDown(SDLK_SPACE) || IsKeyDown(SDL_CONTROLLER_BUTTON_START)) {
			pause = true;
			titleStatus = 2;
			SDL_Delay(200);
		}
		float oldxpos = xpos;
		//Bagian ini mengatur pergerakan dan anim_state megaman. 
		//Kanan kiri untuk berlari. Space tahan meloncat. Tombol 1 tahan tidur.

		//Animasi default adalah animasi idle dengan anim_state 3. 
		if (ypos > floorY) anim_state = 2; else anim_state = 3;

		//Jalan ke kanan.
		if (IsKeyDown(SDLK_RIGHT) || IsKeyDown(SDL_BUTTON_RIGHT)) {
			if (anim_state != 4) { //Ketika tidur tidak bisa bergerak
				flip = 0;
				if (ypos>floorY) { //Ketika jatuh bisa bergerak, tapi menggunakan animasi lompat
					anim_state = 2;
				}else {
					anim_state = 1; //Ketika ditanah, bisa berlari. Animasi lari akan dijalankan. 
				}
				xpos += deltaTime * SPEED; //Mengubah posisi megaman
				if (xpos > 1.0f) { //Mencegah Megaman menembus dinding kanan dengan menetralkan penambahan xpos
					xpos -= deltaTime * SPEED;
				}
			}
		}
		//Jalan ke kiri
		if (IsKeyDown(SDLK_LEFT) || IsKeyDown(SDL_BUTTON_LEFT)) {
			if (anim_state != 4) {
				flip = 1; //Hampir sama dengan jalan kanan, xpos di balik menjadi negatif, sprite di flip.
				if (ypos>floorY) {
					anim_state = 2;
				}
				else {
					anim_state = 1;
				}
				xpos -= deltaTime * SPEED;
				if (xpos < -1.0f) { //Mencegah menembus dinding kiri.
					xpos += deltaTime * SPEED;
				}
			}
		}
		if (ypos>floorY) ypos -= deltaTime * 0.0005f;//Gravitasi setengah kekuatan lompat
		else if (ypos < floorY) { ypos = floorY+0.001f; }//Jika berada di lantai. Akan mencegah menembus lantai.
	
		//Gerakkan melon
		if (IsCollided(xpos, ypos, spriteWidth, spriteWidth, xposFruitMelon1, yposFruitMelon1, crateWidth, crateHeight)) {
			yposFruitMelon1 = 2.0f;
			score += 10;
		}
		else {
			if (yposFruitMelon1< floorY) {
				yposFruitMelon1 = 1.5f;
				xposFruitMelon1 -= 0.3f;
				if (xposFruitMelon1 < -0.9f) {
					xposFruitMelon1 = 0.9f;
				}
			}
			else {
				yposFruitMelon1 -= fruitSpeed;
			}
		}
		//Gerakkan semangka
		if (IsCollided(xpos, ypos, spriteWidth, spriteWidth, xposFruitSemangka1, yposFruitSemangka1, crateWidth, crateHeight)) {
			yposFruitSemangka1 = 2.0f;
			score += 20;
		}
		else {
			if (yposFruitSemangka1< floorY) {
				yposFruitSemangka1 = 1.5f;
				xposFruitSemangka1 += 0.3f;
				if (xposFruitSemangka1 > 1.0f) {
					xposFruitSemangka1 = -1.0f;
				}
			}
			else {
				yposFruitSemangka1 -= fruitSpeed;
			}
		}
	
		//Gerakkan Pisang
		if (IsCollided(xpos, ypos, spriteWidth, spriteWidth, xposFruitPisang1, yposFruitPisang1, crateWidth, crateHeight)) {
			yposFruitPisang1 = 2.0f;
			score += 10;
		}
		else {
			if (yposFruitPisang1< floorY) {
				yposFruitPisang1 = 1.5f;
				xposFruitPisang1 += 0.3f;
				if (xposFruitPisang1 > 1.0f) {
					xposFruitPisang1 = -0.9f;
				}
			}
			else {
				yposFruitPisang1 -= fruitSpeed;
			}
		}

		//Gerakkan Bom1
		if (IsCollided(xpos, ypos, spriteWidth, spriteWidth, xposBom1, yposBom1, crateWidth, crateHeight)) {
			pause = true;
			titleStatus = 3;
			SDL_Delay(150);
		}
		else {
			if (yposBom1< floorY) {
				yposBom1 = 1.5f;
				xposBom1 += 0.3f;
				if (xposBom1 > 1.0f) {
					xposBom1 = -1.0f;
				}
			}
			else {
				yposBom1 -= bombSpeed;
			}
		}
	
		//Gerakkan Bom2
		if (IsCollided(xpos, ypos, spriteWidth, spriteWidth, xposBom2, yposBom2, crateWidth, crateHeight)) {
			pause = true;
			titleStatus = 3;
			SDL_Delay(150);
		}
		else {
			if (yposBom2< floorY) {
				yposBom2 = 1.5f;
				xposBom2 += 0.3f;
				if (xposBom2 > 1.0f) {
					xposBom2 = -1.0f;
				}
			}
			else {
				yposBom2 -= bombSpeed;
			}
		}
	}
	
	if (titleStatus == 1 && pause == true) {
		xposTitle = 0.0f; yposTitle = 0.0f;
		xposPause = 3.0f; yposPause = 3.0f;
		xposGameOver = 3.0f; yposGameOver = 3.0f;
	}
	else if (titleStatus == 2 && pause == true) {
		xposTitle = 3.0f; yposTitle = 3.0f;
		xposPause = 0.0f; yposPause = 0.0f;
		xposGameOver = 3.0f; yposGameOver = 3.0f;
	}
	else if (titleStatus == 3 && pause == true) {
		xposTitle = 3.0f; yposTitle = 3.0f;
		xposPause = 3.0f; yposPause = 3.0f;
		xposGameOver = 0.0f; yposGameOver = 0.0f;
	}
	else if (pause == false) {
		xposTitle = 3.0f; yposTitle = 3.0f;
		xposPause = 3.0f; yposPause = 3.0f;
		xposGameOver = 3.0f; yposGameOver = 3.0f;
	}

	mat4 transformPlayer;
	transformPlayer = translate(transformPlayer, vec3(xpos, ypos, 0.0f));
	GLint location1a = glGetUniformLocation(this->programPlayer, "transform");
	GLint location1b= glGetUniformLocation(this->programPlayer, "flip");
	UseShader(this->programPlayer);
	glUniformMatrix4fv(location1a, 1, GL_FALSE, value_ptr(transformPlayer));
	glUniform1i(location1b, flip);

	mat4 transformFruitMelon1;
	transformFruitMelon1 = translate(transformFruitMelon1, vec3(xposFruitMelon1, yposFruitMelon1, 0.0f));
	GLint locationFruitMelon1 = glGetUniformLocation(this->programFruitMelon1, "transform");
	UseShader(this->programFruitMelon1);
	glUniformMatrix4fv(locationFruitMelon1, 1, GL_FALSE, value_ptr(transformFruitMelon1));

	mat4 transformFruitSemangka1;
	transformFruitSemangka1 = translate(transformFruitSemangka1, vec3(xposFruitSemangka1, yposFruitSemangka1, 0.0f));
	GLint locationFruitSemangka1 = glGetUniformLocation(this->programFruitSemangka1, "transform");
	UseShader(this->programFruitSemangka1);
	glUniformMatrix4fv(locationFruitSemangka1, 1, GL_FALSE, value_ptr(transformFruitSemangka1));

	mat4 transformFruitPisang1;
	transformFruitPisang1 = translate(transformFruitPisang1, vec3(xposFruitPisang1, yposFruitPisang1, 0.0f));
	GLint locationFruitPisang1 = glGetUniformLocation(this->programFruitPisang1, "transform");
	UseShader(this->programFruitPisang1);
	glUniformMatrix4fv(locationFruitPisang1, 1, GL_FALSE, value_ptr(transformFruitPisang1));

	mat4 transformBom1;
	transformBom1 = translate(transformBom1, vec3(xposBom1, yposBom1, 0.0f));
	GLint locationBom1 = glGetUniformLocation(this->programBom1, "transform");
	UseShader(this->programBom1);
	glUniformMatrix4fv(locationBom1, 1, GL_FALSE, value_ptr(transformBom1));

	mat4 transformBom2;
	transformBom2 = translate(transformBom2, vec3(xposBom2, yposBom2, 0.0f));
	GLint locationBom2 = glGetUniformLocation(this->programBom2, "transform");
	UseShader(this->programBom2);
	glUniformMatrix4fv(locationBom2, 1, GL_FALSE, value_ptr(transformBom2));

	mat4 projection = ortho(0.0f, (float)this->screenWidth, 0.0f, (float)this->screenHeight);
	GLint locationTextScore = glGetUniformLocation(this->programTextScore, "projection");
	UseShader(this->programTextScore);
	glUniformMatrix4fv(locationTextScore, 1, GL_FALSE, value_ptr(projection));

	mat4 transformTitle;
	transformTitle = translate(transformTitle, vec3(xposTitle, yposTitle, 0.0f));
	GLint locationTitle = glGetUniformLocation(this->programTitle, "transform");
	UseShader(this->programTitle);
	glUniformMatrix4fv(locationTitle, 1, GL_FALSE, value_ptr(transformTitle));

	mat4 transformPause;
	transformPause = translate(transformPause, vec3(xposPause, yposPause, 0.0f));
	GLint locationPause = glGetUniformLocation(this->programPause, "transform");
	UseShader(this->programPause);
	glUniformMatrix4fv(locationPause, 1, GL_FALSE, value_ptr(transformPause));

	mat4 transformGameOver;
	transformGameOver = translate(transformGameOver, vec3(xposGameOver, yposGameOver, 0.0f));
	GLint locationGO = glGetUniformLocation(this->programGameOver, "transform");
	UseShader(this->programGameOver);
	glUniformMatrix4fv(locationGO, 1, GL_FALSE, value_ptr(transformGameOver));
}

void Demo::reset() {
	score = 0;
	xpos = 0, ypos = floorY,
	xposFruitMelon1 = 0.0f, yposFruitMelon1 = 0.5f,
	xposFruitSemangka1 = -0.5f, yposFruitSemangka1 = 0.5f,
	xposFruitPisang1 = -1.0f, yposFruitPisang1 = 0.5f,
	xposBom1 = 0.5f, yposBom1 = 0.5f,
	xposBom2 = 1.0f, yposBom2 = 0.5f;
}

void Demo::DrawSprite() { //Menggambar sprite
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	//Menggambar background stage. Agar megaman ada di depan background.
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureStage);
	GLint location2 = glGetUniformLocation(this->programStage, "ourTexture");
	// Activate shader
	UseShader(this->programStage);
	glUniform1i(location2, 1);

	// Draw stage
	glBindVertexArray(VAOStage);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar Player.
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, texturePlayer);
	GLint location = glGetUniformLocation(this->programPlayer, "ourTexture");

	// Activate shader
	UseShader(this->programPlayer);
	glUniform1i(location, 0);

	// Draw sprite
	glBindVertexArray(VAOPlayer);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar Bom1. 
	//Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureBom1);
	GLint locationBom1 = glGetUniformLocation(this->programBom1, "ourTexture");
	// Activate shader
	UseShader(this->programBom1);
	glUniform1i(locationBom1, 1);

	//Draw buah
	glBindVertexArray(VAOBom1);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar Bom2. 
	//Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureBom2);
	GLint locationBom2 = glGetUniformLocation(this->programBom2, "ourTexture");
	// Activate shader
	UseShader(this->programBom2);
	glUniform1i(locationBom2, 1);

	//Draw buah
	glBindVertexArray(VAOBom2);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar buah melon1. 
	//Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFruitMelon1);
	GLint locationFruitMelon1 = glGetUniformLocation(this->programFruitMelon1, "ourTexture");
	// Activate shader
	UseShader(this->programFruitMelon1);
	glUniform1i(locationFruitMelon1, 1);

	//Draw buah
	glBindVertexArray(VAOFruitMelon1);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar buah Semangka1. 
	//Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFruitSemangka1);
	GLint locationFruitSemangka1 = glGetUniformLocation(this->programFruitSemangka1, "ourTexture");
	// Activate shader
	UseShader(this->programFruitSemangka1);
	glUniform1i(locationFruitSemangka1, 1);

	//Draw buah
	glBindVertexArray(VAOFruitSemangka1);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar buah Pisang1. 
	//Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureFruitPisang1);
	GLint locationFruitPisang1 = glGetUniformLocation(this->programFruitPisang1, "ourTexture");
	// Activate shader
	UseShader(this->programFruitPisang1);
	glUniform1i(locationFruitPisang1, 1);

	//Draw buah
	glBindVertexArray(VAOFruitPisang1);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar foreground stage. Agar megaman ada di belakang foreground.
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureStageFront);
	GLint location3 = glGetUniformLocation(this->programStageFront, "ourTexture");
	// Activate shader
	UseShader(this->programStageFront);
	glUniform1i(location3, 1);

	// Draw stage foreground
	glBindVertexArray(VAOStageFront);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	
	//draw score
	string text = "Score : ";
	string ScoreText = to_string(score);
	text.append(ScoreText);

	RenderText(text, 25.0f, 380.0f, 1.0f, vec3(0.1, 0.1f, 0.1f));

	//Menggambar Title. 
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureTitle);
	GLint locationTitle = glGetUniformLocation(this->programTitle, "ourTexture");
	// Activate shader
	UseShader(this->programTitle);
	glUniform1i(locationTitle, 1);

	// Draw Title
	glBindVertexArray(VAOTitle);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar Pause. 
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, texturePause);
	GLint locationPause = glGetUniformLocation(this->programPause, "ourTexture");
	// Activate shader
	UseShader(this->programPause);
	glUniform1i(locationPause, 1);

	// Draw Pause
	glBindVertexArray(VAOPause);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	//Menggambar GameOver. 
	// Bind Textures using texture units
	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, textureGameOver);
	GLint locationGameOver = glGetUniformLocation(this->programGameOver, "ourTexture");
	// Activate shader
	UseShader(this->programGameOver);
	glUniform1i(locationGameOver, 1);

	// Draw GameOver
	glBindVertexArray(VAOGameOver);
	glDrawElements(GL_QUADS, 4, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);


	// Unbind texture when done, so we won't accidentily mess up our texture.
	glBindTexture(GL_TEXTURE_2D, 0);
	glDisable(GL_BLEND);
}

void Demo::BuildSprite(string name) //Membuat sprite megaman.
{
	ypos = floorY;
	if (name == "Player") {
	this->programPlayer = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Pass n to shader
		GLint location = glGetUniformLocation(this->programPlayer, "n");
		UseShader(this->programPlayer);
		glUniform1f(location, 1.0f / NUM_FRAMES);

		// Load and create a texture 
		glGenTextures(1, &texturePlayer);
		glBindTexture(GL_TEXTURE_2D, texturePlayer); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

		// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("prinny.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

		// Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width) / NUM_FRAMES; //Membuat frame
		float w = 0.2f;
		float x = frame_width * w / height/2;
		float y = w;
		spriteHeight = 0.2f;
		spriteWidth = 0.1f;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOPlayer);
		glGenBuffers(1, &VBOPlayer);
		glGenBuffers(1, &EBOPlayer);

		glBindVertexArray(VAOPlayer);

		glBindBuffer(GL_ARRAY_BUFFER, VBOPlayer);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPlayer);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}
	else if(name == "Stage"){
		this->programStage = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Load and create a texture 
		glGenTextures(1, &textureStage);
		glBindTexture(GL_TEXTURE_2D, textureStage); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("BackGround.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		float w = 1.0f;
		float x = 1.0f;
		float y = 1.0f;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOStage);
		glGenBuffers(1, &VBOStage);
		glGenBuffers(1, &EBOStage);

		glBindVertexArray(VAOStage);

		glBindBuffer(GL_ARRAY_BUFFER, VBOStage);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOStage);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

		ypos2 = 0.0;
		xpos2 = 0.0;

		mat4 transformStage;
		transformStage = translate(transformStage, vec3(xpos2, ypos2, 0.0f));
		GLint locationStage = glGetUniformLocation(this->programStage, "transform");
		UseShader(this->programStage);
		glUniformMatrix4fv(locationStage, 1, GL_FALSE, value_ptr(transformStage));
	}
	else if (name == "Front") {
		ypos = floorY;
		this->programStageFront = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Load and create a texture 
		glGenTextures(1, &textureStageFront);
		glBindTexture(GL_TEXTURE_2D, textureStageFront); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("BackGroundFront.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		float w = 1.0f;
		float x = 1.0f;
		float y = 1.0f;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOStageFront);
		glGenBuffers(1, &VBOStageFront);
		glGenBuffers(1, &EBOStageFront);

		glBindVertexArray(VAOStageFront);

		glBindBuffer(GL_ARRAY_BUFFER, VBOStageFront);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOStageFront);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

		mat4 transformStageFront;
		transformStageFront = translate(transformStageFront, vec3(xpos3, ypos3, 0.0f));
		GLint locationStageFront = glGetUniformLocation(this->programStageFront, "transform");
		UseShader(this->programStageFront);
		glUniformMatrix4fv(locationStageFront, 1, GL_FALSE, value_ptr(transformStageFront));
	}
	else if (name == "FruitMelon1") {
		this->programFruitMelon1 = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Pass n to shader
		GLint locationFruit = glGetUniformLocation(this->programFruitMelon1, "n");
		UseShader(this->programFruitMelon1);
		glUniform1f(locationFruit, 1.0f / 5);

		// Load and create a texture 
		glGenTextures(1, &textureFruitMelon1);
		glBindTexture(GL_TEXTURE_2D, textureFruitMelon1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("fruit.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


		// Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width) / 5; //Membuat frame
		float w = 0.2f;
		float x = frame_width * w / height / 2;
		float y = w;
		crateHeight = x;
		crateWidth = y;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOFruitMelon1);
		glGenBuffers(1, &VBOFruitMelon1);
		glGenBuffers(1, &EBOFruitMelon1);

		glBindVertexArray(VAOFruitMelon1);

		glBindBuffer(GL_ARRAY_BUFFER, VBOFruitMelon1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFruitMelon1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}
	else if (name == "FruitSemangka1") {
		this->programFruitSemangka1 = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Pass n to shader
		GLint locationFruit = glGetUniformLocation(this->programFruitSemangka1, "n");
		UseShader(this->programFruitSemangka1);
		glUniform1f(locationFruit, 1.0f / 5);

		// Load and create a texture 
		glGenTextures(1, &textureFruitSemangka1);
		glBindTexture(GL_TEXTURE_2D, textureFruitSemangka1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

															 // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("fruit.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


										 // Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width) / 5; //Membuat frame
		float w = 0.2f;
		float x = frame_width * w / height / 2;
		float y = w;
		crateHeight = x;
		crateWidth = y;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOFruitSemangka1);
		glGenBuffers(1, &VBOFruitSemangka1);
		glGenBuffers(1, &EBOFruitSemangka1);

		glBindVertexArray(VAOFruitSemangka1);

		glBindBuffer(GL_ARRAY_BUFFER, VBOFruitSemangka1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFruitSemangka1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}
	else if (name == "FruitPisang1") {
		this->programFruitPisang1 = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Pass n to shader
		GLint locationFruit = glGetUniformLocation(this->programFruitPisang1, "n");
		UseShader(this->programFruitPisang1);
		glUniform1f(locationFruit, 1.0f / 5);

		// Load and create a texture 
		glGenTextures(1, &textureFruitPisang1);
		glBindTexture(GL_TEXTURE_2D, textureFruitPisang1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

														   // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("fruit.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


										 // Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width) / 5; //Membuat frame
		float w = 0.2f;
		float x = frame_width * w / height / 2;
		float y = w;
		crateHeight = x;
		crateWidth = y;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOFruitPisang1);
		glGenBuffers(1, &VBOFruitPisang1);
		glGenBuffers(1, &EBOFruitPisang1);

		glBindVertexArray(VAOFruitPisang1);

		glBindBuffer(GL_ARRAY_BUFFER, VBOFruitPisang1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOFruitPisang1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}
	else if (name == "Bom1") {
		this->programBom1 = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Pass n to shader
		GLint location = glGetUniformLocation(this->programBom1, "n");
		UseShader(this->programBom1);
		glUniform1f(location, 1.0f / 5);

		// Load and create a texture 
		glGenTextures(1, &textureBom1);
		glBindTexture(GL_TEXTURE_2D, textureBom1); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												   // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("fruit.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


										 // Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width) / 5; //Membuat frame
		float w = 0.2f;
		float x = frame_width * w / height / 2;
		float y = w;
		crateHeight = x;
		crateWidth = y;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOBom1);
		glGenBuffers(1, &VBOBom1);
		glGenBuffers(1, &EBOBom1);

		glBindVertexArray(VAOBom1);

		glBindBuffer(GL_ARRAY_BUFFER, VBOBom1);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBom1);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}
	else if (name == "Bom2") {
		this->programBom2 = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Pass n to shader
		GLint location = glGetUniformLocation(this->programBom2, "n");
		UseShader(this->programBom2);
		glUniform1f(location, 1.0f / 5);

		// Load and create a texture 
		glGenTextures(1, &textureBom2);
		glBindTexture(GL_TEXTURE_2D, textureBom2); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

												   // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("fruit.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.


										 // Set up vertex data (and buffer(s)) and attribute pointers
		frame_width = ((float)width) / 5; //Membuat frame
		float w = 0.2f;
		float x = frame_width * w / height / 2;
		float y = w;
		crateHeight = x;
		crateWidth = y;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOBom2);
		glGenBuffers(1, &VBOBom2);
		glGenBuffers(1, &EBOBom2);

		glBindVertexArray(VAOBom2);

		glBindBuffer(GL_ARRAY_BUFFER, VBOBom2);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOBom2);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}
	else if (name == "TextScore") {
		// Init Freetype
		FT_Library ft;
		if (FT_Init_FreeType(&ft)) {
			Err("ERROR::FREETYPE: Could not init FreeType Library");
		}
		FT_Face face;
		if (FT_New_Face(ft, FONTNAME, 0, &face)) {
			Err("ERROR::FREETYPE: Failed to load font");
		}

		FT_Set_Pixel_Sizes(face, 0, FONTSIZE);

		glPixelStorei(GL_UNPACK_ALIGNMENT, 1); // Disable byte-alignment restriction
		for (GLubyte c = 0; c < 128; c++)
		{
			// Load character glyph
			if (FT_Load_Char(face, c, FT_LOAD_RENDER))
			{
				std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
				continue;
			}
			// Generate texture
			GLuint textureTextScore;
			glGenTextures(1, &textureTextScore);
			glBindTexture(GL_TEXTURE_2D, textureTextScore);
			glTexImage2D(
				GL_TEXTURE_2D,
				0,
				GL_RED,
				face->glyph->bitmap.width,
				face->glyph->bitmap.rows,
				0,
				GL_RED,
				GL_UNSIGNED_BYTE,
				face->glyph->bitmap.buffer
				);
			// Set texture options
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			// Now store character for later use
			Character character = {
				textureTextScore,
				ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
				ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
				face->glyph->advance.x
			};
			Characters.insert(pair<GLchar, Character>(c, character));
		}

		FT_Done_Face(face);
		FT_Done_FreeType(ft);

		glGenVertexArrays(1, &VAOTextScore);
		glGenBuffers(1, &VBOTextScore);
		glBindVertexArray(VAOTextScore);
		glBindBuffer(GL_ARRAY_BUFFER, VBOTextScore);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		this->programTextScore = BuildShader("font.vert", "font.frag");
	}
	else if (name == "Title") {
		this->programTitle = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Load and create a texture 
		glGenTextures(1, &textureTitle);
		glBindTexture(GL_TEXTURE_2D, textureTitle); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("Title.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		float w = 1.0f;
		float x = 1.0f;
		float y = 1.0f;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOTitle);
		glGenBuffers(1, &VBOTitle);
		glGenBuffers(1, &EBOTitle);

		glBindVertexArray(VAOTitle);

		glBindBuffer(GL_ARRAY_BUFFER, VBOTitle);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOTitle);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}
	else if (name == "Pause") {
		this->programPause = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Load and create a texture 
		glGenTextures(1, &texturePause);
		glBindTexture(GL_TEXTURE_2D, texturePause); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													// Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("Pause.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		float w = 1.0f;
		float x = 1.0f;
		float y = 1.0f;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOPause);
		glGenBuffers(1, &VBOPause);
		glGenBuffers(1, &EBOPause);

		glBindVertexArray(VAOPause);

		glBindBuffer(GL_ARRAY_BUFFER, VBOPause);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOPause);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO

	}
	else if (name == "GameOver") {
		this->programGameOver = BuildShader("spriteAnim.vert", "spriteAnim.frag");

		// Load and create a texture 
		glGenTextures(1, &textureGameOver);
		glBindTexture(GL_TEXTURE_2D, textureGameOver); // All upcoming GL_TEXTURE_2D operations now have effect on our texture object

													   // Set texture filtering
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

		// Load, create texture 
		int width, height;
		unsigned char* image = SOIL_load_image("GameOver.png", &width, &height, 0, SOIL_LOAD_RGBA);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
		glBindTexture(GL_TEXTURE_2D, 0); // Unbind texture when done, so we won't accidentily mess up our texture.

										 // Set up vertex data (and buffer(s)) and attribute pointers
		float w = 1.0f;
		float x = 1.0f;
		float y = 1.0f;
		GLfloat vertices[] = {
			// Positions   // Colors           // Texture Coords
			x,  y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f, // Top Right
			x, -y, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f, // Bottom Right
			-x, -y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 0.0f, // Bottom Left
			-x,  y, 0.0f,  1.0f, 1.0f, 1.0f,   0.0f, 1.0f  // Top Left 
		};

		GLuint indices[] = {  // Note that we start from 0!
			0, 3, 2, 1
		};

		glGenVertexArrays(1, &VAOGameOver);
		glGenBuffers(1, &VBOGameOver);
		glGenBuffers(1, &EBOGameOver);

		glBindVertexArray(VAOGameOver);

		glBindBuffer(GL_ARRAY_BUFFER, VBOGameOver);
		glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOGameOver);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

		// Position attribute
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), 0);
		glEnableVertexAttribArray(0);
		// Color attribute
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		glEnableVertexAttribArray(1);
		// TexCoord attribute
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(GLfloat), (GLvoid*)(6 * sizeof(GLfloat)));
		glEnableVertexAttribArray(2);

		glBindVertexArray(0); // Unbind VAO
	}
}

bool Demo::IsCollided(float x1, float y1, float width1, float height1, float x2, float y2, float width2, float height2) {
	return (x1 < x2 + width2 && x1 + width1 > x2 && y1 < y2 + height2 && y1 + height1 > y2);
}

void Demo::InitAndPlayMusic() {
	// Init and Play Audio
	ISoundEngine *SoundEngine = createIrrKlangDevice();
	SoundEngine->play2D(this->MP3NAME, GL_TRUE);
}

void Demo::RenderText(string text, GLfloat x, GLfloat y, GLfloat scale, vec3 color)
{
	// Activate corresponding render state
	UseShader(this->programTextScore);
	glUniform3f(glGetUniformLocation(this->programTextScore, "textColor"), color.x, color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(VAOTextScore);

	// Iterate through all characters
	string::const_iterator c;
	for (c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];
		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;
		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = {
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos, ypos, 0.0, 1.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos, ypos + h, 0.0, 0.0 },
			{ xpos + w, ypos, 1.0, 1.0 },
			{ xpos + w, ypos + h, 1.0, 0.0 }
		};
		// Render glyph texture over quad
		glBindTexture(GL_TEXTURE_2D, ch.TextureID);
		// Update content of VBO memory
		glBindBuffer(GL_ARRAY_BUFFER, VBOTextScore);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph (note that advance is number of 1 / 64 pixels)
		x += (ch.Advance >> 6) * scale; // Bitshift by 6 to get value in pixels(2 ^ 6 = 64)
	}
	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

int main(int argc, char** argv) {

	Engine::Game &game = Demo();
	game.Start("Draw Sprite Based Animation", 717, 400, true, WindowFlag::WINDOWED, 0, 1);

	return 0;
}
