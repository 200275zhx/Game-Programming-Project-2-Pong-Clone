#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

class Paddle {
public:
	Paddle(glm::vec3 INIT_POS, glm::vec3 INIT_SCALE, float speed, GLuint textureID);

	void update(float delta_time, bool is_possessed);

	GLuint get_textureID() const;
	glm::mat4 get_model_matrix() const;

	void reset_movement();
	void normalize_movement();
	void move_up();
	void move_down();

	glm::vec3 get_position() const;

private:
	glm::vec3 INIT_POS;
	glm::vec3 INIT_SCALE;

	glm::vec3 position;
	glm::vec3 movement;
	glm::vec3 scale;
	glm::mat4 model_matrix;
	float speed;
	GLuint textureID;
	float auto_move_dir;
};