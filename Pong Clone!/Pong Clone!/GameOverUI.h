#pragma once
#include <GL/glew.h>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <cmath>

class GameOverUI {
public:
	GameOverUI(glm::vec3 INIT_POS, glm::vec3 INIT_SCALE, GLuint textureID);
	void update();

	GLuint get_textureID() const;
	glm::mat4 get_model_matrix() const;

private:
	glm::vec3 INIT_POS;
	glm::vec3 INIT_SCALE;

	glm::mat4 model_matrix;
	GLuint textureID;
};