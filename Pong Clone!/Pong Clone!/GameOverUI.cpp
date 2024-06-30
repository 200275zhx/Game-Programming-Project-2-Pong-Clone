#include "GameOverUI.h"

GameOverUI::GameOverUI(glm::vec3 INIT_POS, glm::vec3 INIT_SCALE, GLuint textureID) :

	INIT_POS(INIT_POS), INIT_SCALE(INIT_SCALE), model_matrix(glm::mat4(1.0f)), textureID(textureID) {}

void GameOverUI::update() {
	// Reset Model Matrix
	model_matrix = glm::mat4(1.0f);

	// Apply INIT Transform
	model_matrix = glm::translate(model_matrix, INIT_POS);
	model_matrix = glm::scale(model_matrix, INIT_SCALE);
}

GLuint GameOverUI::get_textureID() const { return textureID; }
glm::mat4 GameOverUI::get_model_matrix() const { return model_matrix; }