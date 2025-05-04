#include "Enemy.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Camera.h"
#include "Vector2D.h"
#include "Game.h"
#include "CollisionHandler.h"
#include "RigidBody.h"

Enemy::Enemy(Properties* props) : Character(props) {
    m_IsAttacking = false;
    m_AttackCooldown = ENEMY_ATTACK_COOLDOWN;

    m_Collider = new Collider();
    m_Collider->SetBuffer(10, 10, -25, -30);

    m_RigidBody = new RigidBody();
    m_RigidBody->SetFravity(5.0f);

    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 10, 120);
    m_Width = 32;
    m_Height = 26;

    m_State = ENEMY_PATROL;
   // m_SpawnPoint = props->Transform;
   m_SpawnPoint = m_Transform;

    m_TargetPosition = Vector2D(0, 0);
    m_MoveDirection = -1;
    m_PatrolRange = 50.0f;
}

void Enemy::Draw() {
    m_Animation->Draw(m_Transform.X, m_Transform.Y, m_Width, m_Height, m_Flip);
}

void Enemy::Update(float dt) {
    if (m_State == ENEMY_DIE) {
        m_Animation->Update();
        return;
    }

    Vector2D playerPos = Camera::GetInstance()->GetTarget();
    float distance = Vector2D::Distance(m_Transform, playerPos);

    if (distance > 200.0f) {
        m_State = ENEMY_PATROL;
    }
    else if (distance > ENEMY_ATTACK_RANGE) {
        m_State = ENEMY_CHASE;
    }
    else {
        m_State = ENEMY_ATTACK;
    }

    switch (m_State) {
    case ENEMY_PATROL:
        PatrolState(dt);
        break;
    case ENEMY_CHASE:
        ChaseState(dt);
        break;
    case ENEMY_ATTACK:
        AttackState(dt);
        break;
    case ENEMY_IDLE:
        m_RigidBody->UnSetForce();
        break;
    }

    m_RigidBody->Update(dt);

    Vector2D nextPos = m_Transform + m_RigidBody->Position();
    SDL_Rect futureCollider = {
        static_cast<int>(nextPos.X),
        static_cast<int>(nextPos.Y),
        18, 28
    };
    if (!CollisionHandler::GetInstance()->MapCollision(futureCollider)) {
        m_Transform = nextPos;
    } else {
        m_RigidBody->UnSetForce();
    }

    m_Collider->Set(m_Transform.X, m_Transform.Y, 32, 48);

    if (m_State == ENEMY_ATTACK) {
        m_Animation->SetProps("enemy_attack", 1, 6, 100);
    } else if (m_State == ENEMY_CHASE) {
        m_Animation->SetProps("enemy_walk", 1, 10, 120);
    } else if (m_State == ENEMY_PATROL) {
        m_Animation->SetProps("enemy_walk", 1, 10, 150);
    } else {
        m_Animation->SetProps("enemy_idle", 1, 4, 150);
    }

    m_Animation->Update();
}

void Enemy::PatrolState(float dt) {
    float left = m_SpawnPoint.X - m_PatrolRange;
    float right = m_SpawnPoint.X + m_PatrolRange;

    if (m_Transform.X <= left) m_MoveDirection = 1;
    if (m_Transform.X >= right) m_MoveDirection = -1;

    m_RigidBody->AppLyForceX(m_MoveDirection * ENEMY_SPEED);
    m_Flip = (m_MoveDirection > 0) ? SDL_FLIP_NONE : SDL_FLIP_HORIZONTAL;
}

void Enemy::ChaseState(float dt) {
    Vector2D playerPos = Camera::GetInstance()->GetTarget();
    float deltaX = playerPos.X - m_Transform.X;

    if (std::abs(deltaX) > ENEMY_ATTACK_RANGE) {
        if (deltaX > 0) {
            m_RigidBody->AppLyForceX(ENEMY_SPEED);
            m_Flip = SDL_FLIP_NONE;
        } else {
            m_RigidBody->AppLyForceX(-ENEMY_SPEED);
            m_Flip = SDL_FLIP_HORIZONTAL;
        }
    } else {
        m_State = ENEMY_ATTACK;
    }
}

void Enemy::AttackState(float dt) {
    if (m_AttackCooldown > 0) {
        m_AttackCooldown -= dt;
    } else {
        m_IsAttacking = true;
        m_AttackCooldown = ENEMY_ATTACK_COOLDOWN;
    }

    Vector2D playerPos = Camera::GetInstance()->GetTarget();
    if (Vector2D::Distance(m_Transform, playerPos) > ENEMY_ATTACK_RANGE) {
        m_State = ENEMY_CHASE;
    }
}

void Enemy::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}
