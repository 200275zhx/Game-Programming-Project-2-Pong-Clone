#include "Paddle.h"

Paddle::Paddle(glm::vec3 INIT_POS, glm::vec3 INIT_SCALE, float speed, GLuint textureID) :

	INIT_POS(INIT_POS), INIT_SCALE(INIT_SCALE), position(glm::vec3(0.0f)),
	movement(glm::vec3(0.0f)), scale(INIT_SCALE), model_matrix(glm::mat4(1.0f)), speed(speed),
	textureID(textureID), auto_move_dir(1.0f) {}

void Paddle::update(float delta_time, bool is_possessed) {
    // Add direction * units per second * elapsed time
    if (is_possessed) { position += movement * speed * delta_time; } 
    else {
        position += glm::vec3(0.0f, auto_move_dir, 0.0f) * speed * delta_time;
        if (std::abs(position.y) >= 3.75f - 0.5f) { auto_move_dir = -auto_move_dir; }
    }

    model_matrix = glm::mat4(1.0f);

    model_matrix = glm::translate(model_matrix, INIT_POS);
    model_matrix = glm::translate(model_matrix, position);
    model_matrix = glm::scale(model_matrix, INIT_SCALE);
}

GLuint Paddle::get_textureID() const { return textureID; }
glm::mat4 Paddle::get_model_matrix() const { return model_matrix; }

void Paddle::reset_movement() { movement = glm::vec3(0.0f); }
void Paddle::normalize_movement() { if (glm::length(movement) > 1.0f) movement = glm::normalize(movement); }
void Paddle::move_up() { movement.y = 1.0f; }
void Paddle::move_down() { movement.y = -1.0f; }

glm::vec3 Paddle::get_position() const { return position; }