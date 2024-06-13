#include "Game.h"

void Game::onCreate() {
}

void Game::onUpdate(const float deltaTime) {
    m_api->clearFrameBuffer(RHI::Color(0x2D, 0x2D, 0xFF));
    m_api->swapBuffers();
}

void Game::onDestroy() {
}