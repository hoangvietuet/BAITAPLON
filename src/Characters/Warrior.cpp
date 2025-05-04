
#include "Warrior.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Camera.h"
#include "Vector2D.h"
#include "Game.h"
#include "CollisionHandler.h"

Warrior::Warrior(Properties* props) : Character(props) {
    m_IsRunning = false;
    m_IsJumping = false;
    m_IsFalling = false;
    m_IsGrounded = false;
    m_IsAttacking = false;
    m_IsCrouching = false;

    m_Flip = SDL_FLIP_NONE;
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;
    m_AttackTime = ATTACK_TIME;

    m_Collider = new Collider();
    m_Collider->SetBuffer(10, 10, -25, -30);


    m_RigidBody = new RigidBody();
    m_RigidBody->SetFravity(5.0f);

    m_Animation = new Animation();
    m_Animation->SetProps(m_TextureID, 1, 7, 120);
    m_Width = 96;
    m_Height = 84;
}
void Warrior::Draw() {
    m_Animation->Draw(m_Transform.X, m_Transform.Y, m_Width, m_Height, m_Flip);

}


// Cập nhật frame
void Warrior::Update(float dt) {
    m_IsRunning = false;
    m_IsCrouching = false;
    m_RigidBody->UnSetForce();

    m_Animation->SetProps("player", 1, 7, 120);  // Đảm bảo animation đúng khi không chạy hay nhảy

    // Di chuyển về phía trước
    if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == FORWARD && !m_IsAttacking) {
        m_RigidBody->AppLyForceX(FORWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_NONE;
        m_IsRunning = true;
    }
    // Di chuyển lùi
    if (Input::GetInstance()->GetAxisKey(HORIZONTAL) == BACKWARD && !m_IsAttacking) {
        m_RigidBody->AppLyForceX(BACKWARD * RUN_FORCE);
        m_Flip = SDL_FLIP_HORIZONTAL;
        m_IsRunning = true;
    }
    // Ngồi (Crouch)
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_S)) {
        m_RigidBody->UnSetForce();
        m_IsCrouching = true;
    }

     if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_K)) {
    m_RigidBody->UnSetForce();
    m_IsAttacking = true;

    m_Animation->SetProps("player_attack", 1, 6, 120, m_Flip); // Dùng cùng một hình ảnh, nhưng lật theo hướng
}
    // Di chuyển sang trái
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)) {
        m_RigidBody->AppLyForceX(5 * BACKWARD);
        m_Animation->SetProps("player_run", 1, 8, 120, SDL_FLIP_HORIZONTAL);
    }
    // Di chuyển sang phải
    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D)) {
        m_RigidBody->AppLyForceX(5 * FORWARD);
        m_Animation->SetProps("player_run", 1, 8, 120);
    }

   //nhảy
   if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsGrounded) {
    m_IsJumping = true;
    m_IsGrounded = false;
    m_RigidBody->AppLyForceY(UPWARD * m_JumpForce);

    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)) { // Nhảy sang trái
        m_Animation->SetProps("player_jump_left", 1, 5, 120);
        m_Flip = SDL_FLIP_NONE; // Không lật vì đã có ảnh riêng
    } else if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D)) { // Nhảy sang phải
        m_Animation->SetProps("player_jump_right", 1, 5, 120);
        m_Flip = SDL_FLIP_NONE;
    }
}



    if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsJumping && m_JumpTime > 0) {
        m_JumpTime -= dt;
        m_RigidBody->AppLyForceY(UPWARD * m_JumpForce);
    } else {
        m_IsJumping = false;
        m_JumpTime = JUMP_TIME;
    }

    // Rơi
    if (m_RigidBody->Veclocity().Y > 0 && !m_IsGrounded) {
        m_IsFalling = true;
    } else {
        m_IsFalling = false;
    }

    // Thời gian tấn công
    if (m_IsAttacking && m_AttackTime > 0) {
        m_AttackTime -= dt;
    } else {
        m_IsAttacking = false;
        m_AttackTime = ATTACK_TIME;
    }

    m_RigidBody->Update(dt);

    // Kiểm tra va chạm X (tường)
    m_LastSafePosition.X = m_Transform.X;
    m_Transform.X += m_RigidBody->Position().X;
    m_Collider->Set(m_Transform.X + 16, m_Transform.Y + 28, 64, 56);

    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
        m_Transform.X = m_LastSafePosition.X;  // Dừng di chuyển khi va chạm với tường
    }
    // Kiểm tra va chạm Y (đất)
    m_RigidBody->Update(dt);
    m_LastSafePosition.Y = m_Transform.Y;
    m_Transform.Y += m_RigidBody->Position().Y;
    m_Collider->Set(m_Transform.X + 16, m_Transform.Y + 28, 64, 56);

    // Kiểm tra va chạm với đất
    if (CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())) {
            // Cập nhật hộp va chạm để đảm bảo nhân vật không bị kẹt
       m_Collider->Set(m_Transform.X + 16, m_Transform.Y + 28, 64, 56);

        m_IsGrounded = true;
        m_Transform.Y = m_LastSafePosition.Y;  // Dừng rơi khi chạm đất
    } else {
        m_IsGrounded = false;
    }

    // Nếu nhân vật đang nhảy hoặc rơi, chuyển sang hoạt ảnh nhảy
    /*if (m_IsJumping || !m_IsGrounded) {
    if (m_Flip == SDL_FLIP_HORIZONTAL) {
        m_Animation->SetProps("player_jump_left", 1, 5, 120);
    } else {
        m_Animation->SetProps("player_jump_right", 1, 5, 120);
    }
}
*/
// Nhảy
if (Input::GetInstance()->GetKeyDown(SDL_SCANCODE_W) && m_IsGrounded) {
    m_IsJumping = true;
    m_IsGrounded = false;
    m_RigidBody->AppLyForceY(UPWARD * m_JumpForce);
}

// Kiểm tra trạng thái nhảy
if (m_IsJumping) {
    if (m_JumpTime > 0) {
        m_JumpTime -= dt;
        m_RigidBody->AppLyForceY(UPWARD * m_JumpForce);
    } else {
        m_IsJumping = false;  // Kết thúc nhảy khi thời gian nhảy hết
        m_JumpTime = 1.5f; // Đặt lại thời gian nhảy thành 1.5 giây
    }
}
// Rơi
if (m_RigidBody->Veclocity().Y > 0 && !m_IsGrounded) {
    m_IsFalling = true;
} else {
    m_IsFalling = false;
}
// Cập nhật hoạt ảnh nhảy
if (m_IsJumping || m_IsFalling) {
    if (m_Flip == SDL_FLIP_HORIZONTAL) {
        m_Animation->SetProps("player_jump_left", 1, 5, 120);
    } else {
        m_Animation->SetProps("player_jump_right", 1, 5, 120);
    }
}

    // Cập nhật gốc của nhân vật
    m_Origin = std::make_unique<Point>();
    m_Origin->X = m_Transform.X + (m_Width / 2);
    m_Origin->Y = m_Transform.Y + (m_Height / 2);

    m_Animation->Update();

}

// Xóa dữ liệu nhân vật
void Warrior::Clean() {
    TextureManager::GetInstance()->Drop(m_TextureID);
}
