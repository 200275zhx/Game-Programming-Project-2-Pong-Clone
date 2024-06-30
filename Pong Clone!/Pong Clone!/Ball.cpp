#include "Ball.h"

Ball::Ball(glm::vec3 INIT_POS, glm::vec3 INIT_SCALE, glm::vec3 vel, GLuint textureID) :

	INIT_POS(INIT_POS), INIT_SCALE(INIT_SCALE), position(glm::vec3(0.0f)),
	movement(glm::vec3(0.0f)), scale(INIT_SCALE), model_matrix(glm::mat4(1.0f)), vel(vel),
	textureID(textureID), collision_cooldown(glm::vec3(0.0f)) {}

GameStatus Ball::update(float delta_time, glm::vec3 paddle1_position,
	glm::vec3 paddle2_position, float paddle_length) {

	collision_cooldown.x += delta_time;
	collision_cooldown.y += delta_time;

	// Collision Detection
		// Define Top & Bottom Edge
	if (position.y * scale.y >= BOARDER_Y - RADIUS ||
		position.y * scale.y <= -BOARDER_Y + RADIUS) {
		if (collision_cooldown.y > 1.0f) { 
			vel.y = -vel.y; 
			collision_cooldown.y = 0.0f;
		}
	}
	// Define Paddle Collision
	if (position.x * scale.x >= BOARDER_X - 0.4f ||
		position.x * scale.x <= -BOARDER_X + 0.4f) {
		if (std::abs(position.y * scale.y - paddle1_position.y) < paddle_length ||
			std::abs(position.y * scale.y - paddle2_position.y) < paddle_length) {
			if (collision_cooldown.x > 1.0f) { 
				vel.x = -vel.x; 
				collision_cooldown.x = 0.0f;
			}
		}
		else if (std::abs(position.y * scale.y - paddle1_position.y) >= paddle_length &&
			position.x < 0) {
			return RIGHT_WIN;
		}
		else if (std::abs(position.y * scale.y - paddle2_position.y) >= paddle_length && 
			position.x > 0) {
			return LEFT_WIN;
		}
	}

	
	// Movement
	movement = glm::normalize(glm::vec3(1.0f, 1.0f, 0.0f));

	// Reset Model Matrix
	model_matrix = glm::mat4(1.0f);

	// Define Rule
	position += movement * vel * delta_time;

	// Apply INIT Transform
	model_matrix = glm::translate(model_matrix, INIT_POS);
	model_matrix = glm::scale(model_matrix, INIT_SCALE);

	// Apply Transition
	model_matrix = glm::translate(model_matrix, position);
	return PROCESS;
}

GLuint Ball::get_textureID() const { return textureID; }
glm::mat4 Ball::get_model_matrix() const { return model_matrix; }