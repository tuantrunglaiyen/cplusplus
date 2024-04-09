#include "Warrior.h"
#include "TextureManager.h"
#include "SDL.h"
#include "Input.h"
#include "Engine.h"
#include "CollisionHandler.h"
#include "Camera.h"

#include <iostream>

Warrior::Warrior(Properties* props):Character(props)
{
    m_JumpTime = JUMP_TIME;
    m_JumpForce = JUMP_FORCE;

    m_Collider = new Collider();                     //Collinder ở trong Physics.
    m_Collider->SetBuffer(-90, -70, 0, 0);          //thay đổi giá trị buffer để collider không bị lệch so với nhân vật
    m_RigidBody =  new Rigidbody();
    m_RigidBody->SetGravity(5.0f);



     m_Animation= new Animation();
    m_Animation->SetProps(m_TextureID,1,8,80);
    //(m_texture,1 row,8 frames,80ms ,SDL_FLIP_HORIZONTAL); thêm dấu phải trong ngoặc () nữa để flip
}

void Warrior::Draw()
{
    m_Animation->Draw(m_Transform->X,m_Transform->Y,m_Width,m_Height);

    //Hiển thị collider
   /* Vector2D cam = Camera::GetInstance()->GetPosition();
    SDL_Rect box = m_Collider->Get();
    box.x -= cam.X;
    box.y -= cam.Y;
    SDL_RenderDrawRect(Engine::GetInstance()->GetRenderer(), &box);*/
}



void Warrior::Update(float dt)
{
   (!m_TraiPhai) ?  m_Animation->SetProps("player",1,8,100) : m_Animation->SetProps("player",1,8,100,SDL_FLIP_HORIZONTAL);
       SoMoi  = Input::GetInstance()->GetKeyDownTime();


    m_RigidBody->UnSetForce();

    //Run backward
     if( Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)&&m_IsGrounded == true && !Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE))
   {
           m_RigidBody->ApplyForceX(3*BACKWARD);
           m_LastDirection = 1;
           m_TraiPhai = true;

        m_Animation->SetProps("player_run",1,8,100,SDL_FLIP_HORIZONTAL);
   }

   //Run forward
    if( Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D)&&m_IsGrounded == true&& !Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE ))
   {
           m_RigidBody->ApplyForceX(3*FORWARD);
           m_LastDirection = -1;
                      m_TraiPhai = false;


        m_Animation->SetProps("player_run",1,8,100);
   }
   //jump
    if( SoCu != 0 && SoMoi == 0 )
    {
        IsTheKeyReleased = true;
        luu = SoCu;
    }
    else IsTheKeyReleased = false;

    std::cout<<"Somoi "<<SoMoi<<' '<<"SoCu "<<SoCu<<" SoLuu "<<luu<<std::endl;

        //Kiểm tra xem là nhảy lên,nhảy trái hay nhảy phải.
        if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)&&Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D)&&m_IsGrounded)
        {
            IsJumpRight = false;
            IsJumpUp = false;
            IsJumpLeft = true;
        }
        else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)&&Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)&&m_IsGrounded)
            {
                IsJumpLeft = false;
                IsJumpRight = true;
                IsJumpUp = false;

            }
       else if(Input::GetInstance()->GetKeyDown(SDL_SCANCODE_SPACE)&&m_IsGrounded)
            {
                IsJumpUp = true;
                IsJumpLeft = false;
                IsJumpRight = false;
            }


           //Xử lý nhảy
        if(IsJumpUp && luu > 0)
        {
            m_IsJumping = true;
            m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
            luu -=dt;
            m_Animation->SetProps("Jump",1,2,100);
        }
        else if(IsJumpRight && luu > 0)
        {
            m_IsJumping = true;
            m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
            m_RigidBody->ApplyForceX(FORWARD*dt);
            luu -=dt;
            m_Animation->SetProps("Jump",1,2,100);
                     m_LastDirection = 1;

        }

        else if (IsJumpLeft && luu >0)
        {
            m_IsJumping = true;
            m_RigidBody->ApplyForceY(UPWARD*m_JumpForce);
            m_RigidBody->ApplyForceX(BACKWARD*dt);
            luu -=dt;
             m_Animation->SetProps("Jump",1,2,100,SDL_FLIP_HORIZONTAL);
                      m_LastDirection = -1;

        }
        else
        {
            m_IsJumping = false;
        }

        //Xử lý rơi
    if( m_LastDirection == 1 && !m_IsGrounded )   {  m_RigidBody->ApplyForceX(5*BACKWARD);               m_Animation->SetProps("Fall",1,2,100);}
    else if(m_LastDirection == -1&& !m_IsGrounded ) {m_RigidBody->ApplyForceX(5*FORWARD);                       m_Animation->SetProps("Fall",1,2,100,SDL_FLIP_HORIZONTAL);}


if( m_IsGrounded &&!Input::GetInstance()->GetKeyDown(SDL_SCANCODE_A)  && !Input::GetInstance()->GetKeyDown(SDL_SCANCODE_D) )  m_LastDirection = 0;


    //move on x axis


    m_RigidBody->Update(dt);                            // truyền dt vào là thông số thời gian trôi qua để tính vận tốc và vị wtrí
        m_LastSafePosition.X = m_Transform->X;          // lưu lại vị trí an toàn trước khi di chuyển

    m_Transform->X += m_RigidBody->Position().X;       // hàm này để tăng giá trị của x trong m_Transform sau khi thay đỏi m_RigidBody
    //  m_Transform->TranslateY(m_RigidBody->Position().Y);

    m_Collider->Set(m_Transform->X, m_Transform->Y, 18, 50);
    //dùng để kiểm tra va chạm với map. 18,50 là kích thước của collider


    //Kiểm tra va chạm với map. Nếu va chạm với map thì trả lại vị trí ban đầu trước đó.
    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get()))        //Nếu va chạm với map return true
        m_Transform->X = m_LastSafePosition.X;


    // move on Y axis

    m_RigidBody->Update(dt);

    m_LastSafePosition.Y = m_Transform->Y;

    m_Transform->Y += m_RigidBody->Position().Y;

    m_Collider->Set(m_Transform->X, m_Transform->Y, 18, 50);

    if(CollisionHandler::GetInstance()->MapCollision(m_Collider->Get())){
        m_IsGrounded = true;
        m_Transform->Y = m_LastSafePosition.Y;
    }
    else{
        m_IsGrounded = false;
    }


    //Thay đổi m_Origin khi nhân vật di chuyển.
    m_Origin->X = m_Transform->X + m_Width/2;
    m_Origin->Y = m_Transform->Y + m_Height/2;

    m_Animation->Update();              //Cập nhật lại giá trị dt

    SoCu = SoMoi;
}

void Warrior::Clean()
{
    TextureManager::GetInstance() ->Clean();
}
