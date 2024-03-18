#include "Animation.h"
#include "TextureManager.h"

void Animation::Update()
{
    m_SpriteFrame = (SDL_GetTicks() /  m_AimSpeed) % m_FrameCount;      //Chia frame them thời gian
}

void Animation::Draw(float x, float y,int spritWidth, int spriteHeight)
{
    TextureManager::GetInstance()->DrawFrame(m_TextureID, x, y, spritWidth, spriteHeight, m_SpriteRow, m_SpriteFrame, m_Flip);
}

void Animation::SetProps(std::string textureID, int spritRow,int frameCount, int aimSpeed, SDL_RendererFlip flip)
{
    m_TextureID = textureID;
    m_SpriteRow = spritRow;
    m_FrameCount = frameCount;
    m_AimSpeed = aimSpeed;
    m_Flip = flip;
}
