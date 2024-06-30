#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

constexpr float BOARDER_Y = 3.75f;
constexpr float BOARDER_X = 5.0f;
constexpr float RADIUS = 0.2f;
enum GameStatus { PROCESS, LEFT_WIN, RIGHT_WIN };

class Ball {
public:
	Ball(glm::vec3 INIT_POS, glm::vec3 INIT_SCALE, glm::vec3 vel, GLuint textureID);

	GameStatus update(float delta_time, glm::vec3 paddle1_position,
		glm::vec3 paddle2_position, float paddle_length);

	GLuint get_textureID() const;
	glm::mat4 get_model_matrix() const;

private:
	glm::vec3 INIT_POS;
	glm::vec3 INIT_SCALE;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 scale;
	glm::mat4 model_matrix;
	glm::vec3 vel;
	GLuint textureID;

	glm::vec3 collision_cooldown;
};