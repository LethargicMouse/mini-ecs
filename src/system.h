#ifndef _SYSTEM_H_
#define _SYSTEM_H_

#include "component.h"

void gravitySystem(float dt, TransformComponent *transformComponent,
                   SpeedComponent *speedComponent);

#endif
