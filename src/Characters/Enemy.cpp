/*#include "Enemy.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>
#include "Warrior.h"
#include "Collider.h"
#include "CollisionHandler.h"
Enemy::Enemy(Properties* props) : GameObject(props) {

    m_RigidBody = new RigidBody();
    m_RigidBody->SetFravity(3.0f); // Nếu sai chính tả: "SetFravity" -> "SetGravity"

    m_Collider = new Collider();
    m_Collider->SetBuffer(-20, -10, 40, 20);

    // Tải animation
    m_Animations["Idle"] = new Animation();
    m_Animations["Idle"]->SetProps("enemy_idle", 1, 10, 200);
    m_Animations["Idle"]->SetFrameSize(32, 26);

    m_Animations["Walk"] = new Animation();
    m_Animations["Walk"]->SetProps("enemy_walk", 1, 10, 150);
    m_Animations["Walk"]->SetFrameSize(32, 26);

    m_Animations["Attack"] = new Animation();
    m_Animations["Attack"]->SetProps("enemy_attack", 1, 10, 120);
    m_Animations["Attack"]->SetFrameSize(32, 26);

    m_Animations["Die"] = new Animation();
    m_Animations["Die"]->SetProps("enemy_die", 1, 10, 180);
    m_Animations["Die"]->SetFrameSize(32, 26);

    m_State = ENEMY_IDLE;
    m_Speed = 2.0f;
    m_HP = 100;

    // Gán kích thước vẽ theo animation
    m_Width = 32;
    m_Height = 26;

    // Đặt quái ở giữa màn hình (sửa đúng theo X là chiều ngang, Y là chiều dọc)
    m_Transform.X = SCREEN_WIDTH / 2;
    m_Transform.Y = SCREEN_HEIGHT / 2;

    m_PatrolStart = Vector2D(m_Transform.X - 50, m_Transform.Y);
    m_PatrolEnd = Vector2D(m_Transform.X + 50, m_Transform.Y);
    m_MovingRight = true;
    m_Flip = SDL_FLIP_NONE;
//     LoadAnimations();
}

void Enemy::SetAnimation(const std::string& animID) {
    if (m_Animations.find(animID) != m_Animations.end()) {
        m_Animations[animID]->Update();
    }
}

void Enemy::Patrol() {
    if (m_MovingRight) {
        m_RigidBody->AppLyForceX(m_Speed);
        if (m_Transform.X >= m_PatrolEnd.X) {
            m_MovingRight = false;
            m_Flip = SDL_FLIP_HORIZONTAL;
        }
    } else {
        m_RigidBody->AppLyForceX(-m_Speed);
        if (m_Transform.X <= m_PatrolStart.X) {
            m_MovingRight = true;
            m_Flip = SDL_FLIP_NONE;
        }
    }
}

void Enemy::Update(float dt) {
    switch (m_State) {
        case ENEMY_IDLE:
            SetAnimation("Idle");
            break;
        case ENEMY_WALK:
            SetAnimation("Walk");
            Patrol();
            break;
        case ENEMY_ATTACK:
            SetAnimation("Attack");
            break;
        case ENEMY_DIE:
            SetAnimation("Die");
            break;
    }

    m_RigidBody->Update(dt);

    // Di chuyển theo X và kiểm tra va chạm
    m_Transform.X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform.X, m_Transform.Y, m_Width, m_Height);

    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_Transform.X -= m_RigidBody->Position().X; // Undo X
    }

    // Di chuyển theo Y và kiểm tra va chạm
    m_Transform.Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform.X, m_Transform.Y, m_Width, m_Height);

    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_Transform.Y -= m_RigidBody->Position().Y; // Undo Y
    }

    // Lấy Warrior từ game hoặc tham chiếu vào đây, đảm bảo Warrior đã tồn tại
    if (Game::GetInstance()->GetWarrior()->GetState() == WARRIOR_ATTACK &&
        CollisionHandler::GetInstance()->CheckCollision(
            Game::GetInstance()->GetWarrior()->GetCollider()->Get(),
            m_Collider->Get())) {

       if (m_State != ENEMY_DIE) {
    std::cout << "Enemy Health before attack: " << m_HP << std::endl;
      UpdateHealth(-10);
     std::cout << "Enemy Health after attack: " << m_HP << std::endl;

}
    }
}

void Enemy::Draw() {
    std::string currentAnim;
    switch (m_State) {
        case ENEMY_IDLE: currentAnim = "Idle"; break;
        case ENEMY_WALK: currentAnim = "Walk"; break;
        case ENEMY_ATTACK: currentAnim = "Attack"; break;
        case ENEMY_DIE: currentAnim = "Die"; break;
    }

    if (m_Animations.find(currentAnim) != m_Animations.end()) {
        m_Animations[currentAnim]->Draw(
            m_Transform.X, m_Transform.Y,
            m_Width, m_Height, m_Flip
        );
    } else {
        std::cout << "Animation " << currentAnim << " not found!\n";
    }
}


void Enemy::Clean() {
    delete m_RigidBody;
    delete m_Collider;

    for (auto& anim : m_Animations) {
        delete anim.second;
    }

    m_Animations.clear();
}
*/
#include "Enemy.h"
#include "TextureManager.h"
#include "Game.h"
#include <iostream>
#include "Warrior.h"
#include "Collider.h"
#include "CollisionHandler.h"

Enemy::Enemy(Properties* props) : GameObject(props) {

    m_RigidBody = new RigidBody();
    m_RigidBody->SetFravity(3.0f);

    m_Collider = new Collider();
    m_Collider->SetBuffer(-20, -10, 40, 20);

    LoadAnimations();

    m_State = ENEMY_IDLE;
    m_Speed = 2.0f;
    m_HP = 100;

    m_Width = 32;
    m_Height = 26;

    m_Transform.X = SCREEN_WIDTH / 2;
    m_Transform.Y = SCREEN_HEIGHT / 2;

    m_PatrolStart = Vector2D(m_Transform.X - 50, m_Transform.Y);
    m_PatrolEnd = Vector2D(m_Transform.X + 50, m_Transform.Y);
    m_MovingRight = true;
    m_Flip = SDL_FLIP_NONE;
}

void Enemy::LoadAnimations() {
    m_Animations["Idle"] = new Animation();
    m_Animations["Idle"]->SetProps("enemy_idle", 1, 10, 200);
    m_Animations["Idle"]->SetFrameSize(32, 26);

    m_Animations["Walk"] = new Animation();
    m_Animations["Walk"]->SetProps("enemy_walk", 1, 10, 150);
    m_Animations["Walk"]->SetFrameSize(32, 26);

    m_Animations["Attack"] = new Animation();
    m_Animations["Attack"]->SetProps("enemy_attack", 1, 10, 120);
    m_Animations["Attack"]->SetFrameSize(32, 26);

    m_Animations["Die"] = new Animation();
    m_Animations["Die"]->SetProps("enemy_die", 1, 10, 180);
    m_Animations["Die"]->SetFrameSize(32, 26);
}

void Enemy::SetAnimation(const std::string& animID) {
    if (m_Animations.find(animID) != m_Animations.end()) {
        m_Animations[animID]->Update();
    }
}

void Enemy::Patrol() {
    if (m_MovingRight) {
        m_RigidBody->AppLyForceX(m_Speed);
        if (m_Transform.X >= m_PatrolEnd.X) {
            m_MovingRight = false;
            m_Flip = SDL_FLIP_HORIZONTAL;
        }
    } else {
        m_RigidBody->AppLyForceX(-m_Speed);
        if (m_Transform.X <= m_PatrolStart.X) {
            m_MovingRight = true;
            m_Flip = SDL_FLIP_NONE;
        }
    }
}

void Enemy::Update(float dt) {
    switch (m_State) {
        case ENEMY_IDLE:
            SetAnimation("Idle");
            break;
        case ENEMY_WALK:
            SetAnimation("Walk");
            Patrol();
            break;
        case ENEMY_ATTACK:
            SetAnimation("Attack");
            break;
        case ENEMY_DIE:
            SetAnimation("Die");
            break;
    }

    m_RigidBody->Update(dt);

    m_Transform.X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform.X, m_Transform.Y, m_Width, m_Height);
    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_Transform.X -= m_RigidBody->Position().X;
    }

    m_Transform.Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform.X, m_Transform.Y, m_Width, m_Height);
    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_Transform.Y -= m_RigidBody->Position().Y;
    }

}

void Enemy::UpdateHealth(int damage) {
    m_HP += damage;
    if (m_HP <= 0) {
        m_HP = 0;
        SetState(ENEMY_DIE);
    }
}

int Enemy::GetHP() const {
    return m_HP;
}

void Enemy::Draw() {
    std::string currentAnim;
    switch (m_State) {
        case ENEMY_IDLE: currentAnim = "Idle"; break;
        case ENEMY_WALK: currentAnim = "Walk"; break;
        case ENEMY_ATTACK: currentAnim = "Attack"; break;
        case ENEMY_DIE: currentAnim = "Die"; break;
    }

    if (m_Animations.find(currentAnim) != m_Animations.end()) {
        m_Animations[currentAnim]->Draw(
            m_Transform.X, m_Transform.Y,
            m_Width, m_Height, m_Flip
        );
    } else {
        std::cout << "Animation " << currentAnim << " not found!\n";
    }
}

void Enemy::Clean() {
    delete m_RigidBody;
    delete m_Collider;

    for (auto& anim : m_Animations) {
        delete anim.second;
    }

    m_Animations.clear();
}
