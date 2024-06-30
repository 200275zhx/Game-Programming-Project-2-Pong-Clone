#define GL_SILENCE_DEPRECATION
#define LOG(argument) std::cout << argument << '\n'

#ifdef _WINDOWS
#include <GL/glew.h>
#endif

#define GL_GLEXT_PROTOTYPES 1
#include <SDL.h>
#include "glm/gtc/matrix_transform.hpp"
#include "ShaderProgram.h"
#include <ctime>
#include "general_functions.h"
#include "Ball.h"
#include <vector>
#include "GameOverUI.h"
#include "Paddle.h"

// --------------------- CONSTANTS --------------------- //

enum AppStatus { RUNNING, TERMINATED };

constexpr float WINDOW_SIZE_MULT = 1.0f;

constexpr int WINDOW_WIDTH = 640 * WINDOW_SIZE_MULT,
WINDOW_HEIGHT = 480 * WINDOW_SIZE_MULT;

constexpr float BG_RED = 0.9765625f,
BG_GREEN = 0.47265625f,
BG_BLUE = 0.4609375f,
BG_OPACITY = 1.0f;

constexpr int VIEWPORT_X = 0,
VIEWPORT_Y = 0,
VIEWPORT_WIDTH = WINDOW_WIDTH,
VIEWPORT_HEIGHT = WINDOW_HEIGHT;
constexpr float CANVAS_HEIGHT = 3.75;
constexpr float CANVAS_WIDTH = 5.0f;

constexpr float MILLISECONDS_IN_SECOND = 1000.0;

constexpr float MINIMUM_COLLISION_DISTANCE = 1.0f;

// --------------------- FILE PATH --------------------- //

constexpr char V_SHADER_PATH[] = "shaders/vertex_textured.glsl",
               F_SHADER_PATH[] = "shaders/fragment_textured.glsl";

constexpr char PADDLE_FILEPATH[] = "assets/paddle.png",
               BALL_FILEPATH[] = "assets/ball.png",
               LEFTWIN_FILEPATH[] = "assets/LEFT_WIN.png",
               RIGHTWIN_FILEPATH[] = "assets/RIGHT_WIN.png";

// --------------------- GLOBAL VARIABLES --------------------- //

SDL_Window* g_display_window;
glm::mat4 g_view_matrix, g_projection_matrix;

AppStatus g_app_status = RUNNING;
GameStatus g_game_status = PROCESS;
ShaderProgram g_shader_program = ShaderProgram();

GameOverUI* g_winner_UI = nullptr;

std::vector<Ball*> g_balls;
std::vector<Paddle*> g_paddles;

float g_previous_ticks = 0.0f;
bool g_is_single_player;
unsigned g_ball_amount = 1;

// --------------------- FUNCTIONS --------------------- //

void initialise();
void process_input();
void update();
void render();
void shutdown();

void draw_object(glm::mat4& object_model_matrix, GLuint& object_texture_id)
{
    g_shader_program.set_model_matrix(object_model_matrix);
    glBindTexture(GL_TEXTURE_2D, object_texture_id);
    glDrawArrays(GL_TRIANGLES, 0, 6); 
}

// --------------------- INITIALIZE --------------------- //

void initialise()
{
    SDL_Init(SDL_INIT_VIDEO);
    g_display_window = SDL_CreateWindow("PONG!",
        SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
        WINDOW_WIDTH, WINDOW_HEIGHT,
        SDL_WINDOW_OPENGL);

    SDL_GLContext context = SDL_GL_CreateContext(g_display_window);
    SDL_GL_MakeCurrent(g_display_window, context);


    if (g_display_window == nullptr)
    {
        shutdown();
    }
#ifdef _WINDOWS
    glewInit();
#endif

    glViewport(VIEWPORT_X, VIEWPORT_Y, VIEWPORT_WIDTH, VIEWPORT_HEIGHT);

    g_shader_program.load(V_SHADER_PATH, F_SHADER_PATH);

    g_view_matrix = glm::mat4(1.0f);
    g_projection_matrix = glm::ortho(-CANVAS_WIDTH, CANVAS_WIDTH, -CANVAS_HEIGHT, CANVAS_HEIGHT, -10.0f, 10.0f);

    g_shader_program.set_projection_matrix(g_projection_matrix);
    g_shader_program.set_view_matrix(g_view_matrix);

    glUseProgram(g_shader_program.get_program_id());

    glClearColor(BG_RED, BG_BLUE, BG_GREEN, BG_OPACITY);

    // -------------- INITIALIZE GAME OBJECTS -------------- //

    for (unsigned index{}; index < 3; ++index) {
        Ball* new_ball = new Ball(glm::vec3(0.0f), glm::vec3(0.3f),
            glm::vec3(5.0f, 5.0f, 0.0f), load_texture(BALL_FILEPATH));
        g_balls.push_back(new_ball);
    }
    Paddle* paddle1 = new Paddle(glm::vec3(-4.8f, 0.0f, 0.0f), 
        glm::vec3(2.0f), 1.0f, load_texture(PADDLE_FILEPATH));
    Paddle* paddle2 = new Paddle(glm::vec3(4.8f, 0.0f, 0.0f),
        glm::vec3(2.0f), 1.0f, load_texture(PADDLE_FILEPATH));
    g_paddles.push_back(paddle1);
    g_paddles.push_back(paddle2);

    // enable blending
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// --------------------- PROCESS INPUT --------------------- //

void process_input()
{
    for (Paddle* paddle : g_paddles) { paddle->reset_movement(); }

    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            // End game
        case SDL_QUIT:
        case SDL_WINDOWEVENT_CLOSE:
            g_app_status = TERMINATED;
            break;

        default:
            break;
        }

        // Press t Switch Player Mode
        if (event.type == SDL_KEYDOWN && event.key.keysym.scancode == SDL_SCANCODE_T) {
            g_is_single_player ? g_is_single_player = false : g_is_single_player = true;
        }
    }


    const Uint8* key_state = SDL_GetKeyboardState(NULL);
    
    // ---------- LEFT SIDE PADDLE CONTROL ----------- //

    if (key_state[SDL_SCANCODE_W])
    {
        g_paddles[0]->move_up();
    }
    else if (key_state[SDL_SCANCODE_S])
    {
        g_paddles[0]->move_down();
    }
    
    // ---------- RIGHT SIDE PADDLE CONTROL ----------- //

    if (!g_is_single_player) {
        if (key_state[SDL_SCANCODE_UP])
        {
            g_paddles[1]->move_up();
        }
        else if (key_state[SDL_SCANCODE_DOWN])
        {
            g_paddles[1]->move_down();
        }
    }

    // ---------- SWITCH BALL AMOUNT ----------- //

    if (key_state[SDL_SCANCODE_1]) {
        g_ball_amount = 1;
    } 
    else if (key_state[SDL_SCANCODE_2]) {
        g_ball_amount = 2;
    }
    else if (key_state[SDL_SCANCODE_3]) {
        g_ball_amount = 3;
    }

    // Normalize Movement
    for (Paddle* paddle : g_paddles) { paddle->normalize_movement(); }
}

// --------------------- UPDATE --------------------- //

void update()
{
    // Update Delta Time
    float ticks = (float)SDL_GetTicks() / MILLISECONDS_IN_SECOND;
    float delta_time = ticks - g_previous_ticks;
    g_previous_ticks = ticks;

    if(g_game_status == PROCESS) {
        g_paddles[0]->update(delta_time, true);
        g_paddles[1]->update(delta_time, !g_is_single_player);
        
        for (unsigned index{}; index < g_ball_amount; ++index) {
            g_game_status = g_balls[index]->update
            (delta_time, g_paddles[0]->get_position(), g_paddles[1]->get_position(), 0.5f);

            if (g_game_status == LEFT_WIN) {
                g_winner_UI = new GameOverUI(glm::vec3(0.0f), glm::vec3(4.0f), load_texture(LEFTWIN_FILEPATH));
                break;
            }
            else if (g_game_status == RIGHT_WIN) {
                g_winner_UI = new GameOverUI(glm::vec3(0.0f), glm::vec3(4.0f), load_texture(RIGHTWIN_FILEPATH));
                break;
            }
        }
    }
    else { g_winner_UI->update(); }
}

// --------------------- RENDER --------------------- //

void render() {
    glClear(GL_COLOR_BUFFER_BIT);

    // Vertices
    float vertices[] = {
        -0.5f, -0.5f, 0.5f, -0.5f, 0.5f, 0.5f,  // triangle 1
        -0.5f, -0.5f, 0.5f, 0.5f, -0.5f, 0.5f   // triangle 2
    };

    // Textures
    float texture_coordinates[] = {
        0.0f, 1.0f, 1.0f, 1.0f, 1.0f, 0.0f,     // triangle 1
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 0.0f,     // triangle 2
    };

    glVertexAttribPointer(g_shader_program.get_position_attribute(), 2, GL_FLOAT, false, 0, vertices);
    glEnableVertexAttribArray(g_shader_program.get_position_attribute());

    glVertexAttribPointer(g_shader_program.get_tex_coordinate_attribute(), 2, GL_FLOAT, false, 0, texture_coordinates);
    glEnableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    // Bind texture
    if (g_winner_UI) {
        glm::mat4 model_matrix = g_winner_UI->get_model_matrix();
        GLuint textureID = g_winner_UI->get_textureID();
        draw_object(model_matrix, textureID);
        SDL_GL_SwapWindow(g_display_window);
        return;
    }

    for (Paddle* paddle : g_paddles) {
        glm::mat4 model_matrix = paddle->get_model_matrix();
        GLuint textureID = paddle->get_textureID();
        draw_object(model_matrix, textureID);
    }

    for (unsigned index{}; index < g_ball_amount; ++index) {
        glm::mat4 model_matrix = g_balls[index]->get_model_matrix();
        GLuint textureID = g_balls[index]->get_textureID();
        draw_object(model_matrix, textureID);
    }

    // We disable two attribute arrays now
    glDisableVertexAttribArray(g_shader_program.get_position_attribute());
    glDisableVertexAttribArray(g_shader_program.get_tex_coordinate_attribute());

    SDL_GL_SwapWindow(g_display_window);
}

// --------------------- SHUTDOWN --------------------- //

void shutdown() { SDL_Quit(); }

// --------------------- MAIN --------------------- //

int main(int argc, char* argv[])
{
    initialise();

    while (g_app_status == RUNNING)
    {
        process_input();
        update();
        render();
    }

    shutdown();
    return 0;
}
