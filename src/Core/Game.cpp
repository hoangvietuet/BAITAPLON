#include "Game.h"
#include "TextureManager.h"
#include "Input.h"

#include "Warrior.h"
#include"SDL.h"
#include "Timer.h"
#include "MapParser.h"
#include <iostream>
#include "Camera.h"
#include "CollisionHandler.h"
#include "AnimatedSprite.h"
#include "Enemy.h"
#include <cstdlib>
#include "RigidBody.h"
#include <ctime>
const int groundY = 500;
Game* Game::s_Instance = nullptr;
Warrior* player = nullptr;
bool Game::Init()
{

   if(SDL_Init(SDL_INIT_VIDEO)!= 0 && IMG_Init(IMG_INIT_JPG | IMG_INIT_PNG)!= 0){
     SDL_Log("Failed", SDL_GetError());
    return false;
   }
   SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
   m_Window = SDL_CreateWindow("Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, window_flags); // ưidow flags
   if(m_Window == nullptr){
     SDL_Log("Failed", SDL_GetError());
    return false;
   }

   m_Renderer = SDL_CreateRenderer(m_Window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
   if(m_Renderer == nullptr){
      SDL_Log("Failed Renderer", SDL_GetError());
    return false;
   }

    SDL_Delay(100);
    ShowSplashScreen(m_Renderer);

   if(MapParser::GetInstance()->Load()){
     std::cout << "failed to load map" << std::endl;
   }

TextureManager::GetInstance()->ParseTextures("assets/textures.tml");
/*
TextureManager::GetInstance()->Load("player", "assets/IDLE.png");
TextureManager::GetInstance()->Load("player_run", "assets/RUN.png");
TextureManager::GetInstance()->Load("player_jump_right", "assets/JUMP-RIGHT.png");
TextureManager::GetInstance()->Load("player_jump_left", "assets/JUMP-LEFT.png");
TextureManager::GetInstance()->Load("player_attack", "assets/ATTACK 1.png");

TextureManager::GetInstance()->Load("bg", "assets/BG1.png");
TextureManager::GetInstance()->Load("bg2", "assets/BG2.png");
TextureManager::GetInstance()->Load("bg3", "assets/BG3.png");

TextureManager::GetInstance()->Load("enemy_idle","assets/dung.png");
TextureManager::GetInstance()->Load("enemy_walk","assets/walk.png");
TextureManager::GetInstance()->Load("enemy_attack","assets/attack.png");
TextureManager::GetInstance()->Load("enemy_die","assets/die.png");
*/



player = new Warrior(new Properties("player", 100, 200, 136, 96));
m_LevelMap = MapParser::GetInstance()->GetMap("level1");
 if (m_Enemy) {
    std::cout << "Enemy initialized successfully!" << std::endl;
} else {
    std::cout << "Failed to initialize enemy!" << std::endl;
}
   //tf.Log();
    Camera::GetInstance()->SetTarget(player->GetOrigin());
   return m_IsRunning = true;
}

bool Game::Clean()
{
    for (auto enemy : m_Enemies) {
        enemy->Clean();
        delete enemy;
    }
    m_Enemies.clear();
  TextureManager::GetInstance()->Clean();
  SDL_DestroyRenderer(m_Renderer);
  SDL_DestroyWindow(m_Window);
  IMG_Quit();
  SDL_Quit();

  if (player) {
    player->Clean();
    delete player;
    player = nullptr;
}

 return true;
}

void Game::Quit()
{
  m_IsRunning = false;
}

void Game::Render() {
    std::cout << "Render function called..." << std::endl;
SDL_SetRenderDrawColor(m_Renderer, 0, 0, 0, 255);
SDL_RenderClear(m_Renderer);

TextureManager::GetInstance()->Draw("bg", 0, 0, 1920, 1080, 5, 5, 0.4);
m_LevelMap->Render();
player->Draw();

     for (auto enemy : m_Enemies) {
        enemy->Draw();
    }

SDL_RenderPresent(m_Renderer);  // Hiển thị tất cả nội dung lên màn hình

}

void Game::Update() {
    float dt = Timer::GetInstance()->GetDeltaTime();

    // Cập nhật nhân vật
    player->Update(dt);

    // Cập nhật thời gian sinh quái
    m_EnemySpawnTimer += dt;

    // Kiểm tra điều kiện để sinh quái
    if (m_EnemySpawnTimer >= m_EnemySpawnInterval && m_Enemies.size() < MAX_ENEMIES) {
        m_EnemySpawnTimer = 0.0f;

        // Lấy vị trí nhân vật
        int offset = rand() % 200 - 100; // từ -100 đến +100
int spawnX = player->GetX() + offset;
int spawnY = player->GetY() - 100;
        std::cout << "Spawning enemy at: X=" << spawnX << " Y=" << spawnY << std::endl;

        // Tạo quái vật mới
        Enemy* newEnemy = new Enemy(new Properties("enemy_idle", spawnX, spawnY, 64, 64));
        m_Enemies.push_back(newEnemy);
    }

    // Cập nhật tất cả quái vật
    for (auto enemy : m_Enemies) {
        enemy->Update(dt);
    }

    // Cập nhật camera theo nhân vật
    Camera::GetInstance()->SetTarget(player->GetOrigin());
    Camera::GetInstance()->Update(dt);

    // Cập nhật map
    m_LevelMap->Update();
}


void Game::Events()
{
    Input::GetInstance()->Listen();
}
void Game::ShowSplashScreen(SDL_Renderer* renderer) {
    const int numFrames = 7;
    const int frameDuration = 100;
    SDL_Texture* frames[numFrames];

    for (int i = 0; i < numFrames; ++i) {
        std::string path = "assets/splash_" + std::to_string(i) + ".png";
        SDL_Surface* surface = IMG_Load(path.c_str());
        if (!surface) {
            SDL_Log("Failed to load frame %d: %s", i, IMG_GetError());
            return;
        }
        frames[i] = SDL_CreateTextureFromSurface(renderer, surface);
        SDL_FreeSurface(surface);
    }

    bool done = false;
    SDL_Event event;
    int currentFrame = 0;
    Uint32 lastTime = SDL_GetTicks();

    while (!done) {
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) exit(0);
            if (event.type == SDL_KEYDOWN || event.type == SDL_MOUSEBUTTONDOWN) {
                done = true;
            }
        }

        Uint32 now = SDL_GetTicks();
        if (now - lastTime >= frameDuration) {
            currentFrame = (currentFrame + 1) % numFrames;
            lastTime = now;
        }

        SDL_RenderClear(renderer);
        SDL_RenderCopy(renderer, frames[currentFrame], nullptr, nullptr);
        SDL_RenderPresent(renderer);

        SDL_Delay(16);  // ~60 FPS
        if (now - lastTime > 3000) {
            done = true;
        }
    }

    for (int i = 0; i < numFrames; ++i) {
        SDL_DestroyTexture(frames[i]);
    }
}
