#ifndef GAME_H
#define GAME_H

#include "SDL.h"
#include "SDL_image.h"
#include "GameMap.h"
#include "MapParser.h"
#include "Warrior.h"
#include"Enemy.h"
#include <vector>
#include <cstdlib>
#include <ctime>
#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

class Game
{

    public:
        static Game* GetInstance(){
          return s_Instance = (s_Instance != nullptr)? s_Instance: new Game();
        }
    bool Init();
    bool Clean();
    void Quit();

    void Update();
    void Render();
    void Events();

    void SpawnEnemy();
    inline GameMap* GetMap(){return m_LevelMap;}
    inline bool IsRunning(){return m_IsRunning;}
    inline SDL_Renderer* GetRenderer(){return m_Renderer;}
    void ShowSplashScreen(SDL_Renderer* renderer);

    private:
        Game(){}
        bool m_IsRunning;

        GameMap* m_LevelMap;
        SDL_Window* m_Window;
        SDL_Renderer* m_Renderer;
        static Game* s_Instance;
       Warrior* player = nullptr;
        Enemy* m_Enemy;
          std::vector<Enemy*> m_Enemies;
    float m_EnemySpawnTimer = 0.0f;
    const float m_EnemySpawnInterval = 300.0f;
    const int MAX_ENEMIES = 10;

};

#endif // GAME_H
