#ifndef ENEMY_H
#define ENEMY_H

#include "Character.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector2D.h"

#define ENEMY_SPEED 5.0f
#define ENEMY_ATTACK_RANGE 100.0f
#define ENEMY_ATTACK_COOLDOWN 1.0f

enum EnemyState {
    ENEMY_IDLE,
    ENEMY_PATROL,
    ENEMY_CHASE,
    ENEMY_ATTACK,
    ENEMY_DIE
};

class Enemy : public Character {
public:
    Collider* GetCollider() { return m_Collider; }

    Enemy(Properties* props);
    void Draw() override;
    void Update(float dt) override;
    void Clean() override;
    void TakeDamage(int damage);
    Vector2D GetPosition() const { return m_Transform; }
    void SetState(EnemyState state) { m_State = state; }
    EnemyState GetState() const { return m_State; }

private:
    void PatrolState(float dt);
    void ChaseState(float dt);
    void AttackState(float dt);

private:
    bool m_IsAttacking;
    float m_AttackCooldown;

    Collider* m_Collider;

    Animation* m_Animation;
    RigidBody* m_RigidBody;

    Vector2D m_Transform;
    Vector2D m_TargetPosition;
    EnemyState m_State;

    int m_HP;
    int m_MoveDirection; // -1 = trái, 1 = phải
    Vector2D m_SpawnPoint;
float m_PatrolRange = 50.0f;


};

#endif // ENEMY_H
