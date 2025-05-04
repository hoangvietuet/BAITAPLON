/*
#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <map>

#include "GameObject.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collider.h"

enum EnemyState {
    ENEMY_IDLE,
    ENEMY_WALK,
    ENEMY_ATTACK,
    ENEMY_DIE
};

class Enemy : public GameObject {
public:
    Enemy GetOrigin() const;

          Collider* m_Collider;

      Enemy(Properties* props);
    void UpdateHealth(int damage);
    int GetHP() const;
    int GetHealth() const { return m_HP; }

    virtual void Draw() override;
    virtual void Update(float dt) override;
    virtual void Clean() override;

    void SetState(EnemyState state) { m_State = state; }
    EnemyState GetState() const { return m_State; }

private:
    void Patrol();
    void SetAnimation(const std::string& animID);
    void LoadAnimations(); // Thêm phương thức tải hoạt ảnh

    RigidBody* m_RigidBody;

    std::map<std::string, Animation*> m_Animations;
    EnemyState m_State;

    float m_Speed;
    int m_HP;
    bool m_MovingRight;
    Vector2D m_PatrolStart;
    Vector2D m_PatrolEnd;

    Properties* m_Props; // Lưu lại props
};

#endif // ENEMY_H
*/
#ifndef ENEMY_H
#define ENEMY_H

#include <string>
#include <map>

#include "GameObject.h"
#include "Animation.h"
#include "RigidBody.h"
#include "Collider.h"
#include "Vector2D.h"
enum EnemyState {
    ENEMY_IDLE,
    ENEMY_WALK,
    ENEMY_ATTACK,
    ENEMY_DIE
};

class Enemy : public GameObject {
public:
    //Vector2D GetPosition() const { return m_Transform; }
    Enemy(Properties* props);
    void UpdateHealth(int damage);
    int GetHP() const;
    int GetHealth() const { return m_HP; }

    virtual void Draw() override;
    virtual void Update(float dt) override;
    virtual void Clean() override;

    void SetState(EnemyState state) { m_State = state; }
    EnemyState GetState() const { return m_State; }

    Collider* m_Collider;

private:
    void Patrol();
    void SetAnimation(const std::string& animID);
    void LoadAnimations();

    RigidBody* m_RigidBody;
    std::map<std::string, Animation*> m_Animations;
    EnemyState m_State;

    float m_Speed;
    int m_HP;
    bool m_MovingRight;
    Vector2D m_PatrolStart;
    Vector2D m_PatrolEnd;

    Properties* m_Props;
};

#endif // ENEMY_H
