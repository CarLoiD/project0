#ifndef GAME_H
#define GAME_H

#include <ZxEngine/AppBase.h>

class Game final : public Application::AppBase {
public:
    virtual void onCreate() override;
    virtual void onUpdate(const float deltaTime) override;
    virtual void onDestroy() override;
};

#endif // GAME_H