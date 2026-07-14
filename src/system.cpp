#include "system.h"

void gravitySystem(float dt, TransformComponent *transformComponent,
                   SpeedComponent *speedComponent) {
  speedComponent->speed.y += 10 * dt;
  transformComponent->pos += speedComponent->speed * dt;
}
