#include "Dragon.h"
#include <iostream>

static const double airResistance = 0.997;
static const double landResistance = 0.9;
static const double flightCost = 15.;
static const double staminaRegenTime = 5.;

static Point2D hindLegInit() {
    return Point2D(-30, 50);
}
static Point2D foreLegInit() {
    return Point2D(30, 50);
}
Dragon::Dragon() :
    save(0, 0, 0, DragonStats()),
    worldPosition(0, 0),
    mode(Flying),
    foreLeg(foreLegInit()),
    hindLeg(hindLegInit()),
    currentRotation(),
    jumpTime()
{
    currentStamina = save.stats.stamina;
    currentHealth = save.stats.size;
    currentCooldown = save.stats.fireCooldown;
}

Dragon::Dragon(GameSave const& newSave) :
    worldPosition(0, 0),
    mode(Flying),
    foreLeg(foreLegInit()),
    hindLeg(hindLegInit()),
    save(newSave),
    currentRotation(),
    jumpTime()
{
    currentStamina = save.stats.stamina;
    currentHealth = save.stats.size;
    currentCooldown = save.stats.fireCooldown;
}

void Dragon::assignHeightmap(GroundHeightmap *newGround) {
    ground = newGround;
}

ScreenFlipMode Dragon::physicsStep(InputState const& input, Point2D const& screenCorner, std::vector<Bullet> bulletList) {

    bool shouldJump(false);
    for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
    {
        ALLEGRO_EVENT const& event(*it);
        if(event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_W)
        {
            shouldJump = true;
        }
    }
    if(currentCooldown < save.stats.fireCooldown && !isBreathing) {
        currentCooldown += (1./60.);
    }
    if(isBreathing) {
        currentCooldown -= (save.stats.fireCooldown / 2) * (1./60.);
        if(currentCooldown <= 0) {
            currentCooldown = 0;
            isBreathing = false;
        }
    }
    
    currentSkin = &baby;
    switch (mode) {
        case Flying:
        {
            velocity.y += (392.4 / 60.);
            
            if(shouldJump && currentStamina > flightCost)
            {
                velocity.y -= 100 * save.stats.wingspan;
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                    velocity.x += 50 * save.stats.wingspan;
                    velocity.y += 50 * save.stats.wingspan;
                }
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                    velocity.x -= 50 * save.stats.wingspan;
                    velocity.y += 50 * save.stats.wingspan;
                }
                currentStamina -= flightCost;
            }
            
            if(currentStamina <= 0 || al_key_down(&input.keyboardState, ALLEGRO_KEY_S)) {
                // Rotate downwards, towards velocity
            } else if(velocity.y > 0) {
                bool velocityRedirection = velocity.x > 0;
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                    velocityRedirection = true;
                }
                if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                    velocityRedirection = false;
                }
                Point2D addedVelocity(velocityRedirection ? (velocity.y * 0.1) : -(velocity.y * 0.1), 0);
                Point2D relativeMousePosition(al_get_mouse_state_axis(&input.mouseState, 0),
                    al_get_mouse_state_axis(&input.mouseState, 1));
                relativeMousePosition += screenCorner;
                relativeMousePosition -= worldPosition;
                Point2D resultantVelocity = normalised(relativeMousePosition) * magnitude(addedVelocity);
                
                velocity += resultantVelocity;
                velocity.y *= 0.9;
                currentStamina -= (10. / 60.);
            }
            
            
            velocity *= airResistance;
            
            worldPosition += velocity / 60.;
            
            // Use al_transforms to modify the leg positions with rotation (later)
            Point2D actualForelegPoint(worldPosition + foreLeg);
            Point2D actualHindlegPoint(worldPosition + hindLeg);
            if((ground->getInterpolatedWorldPoint(actualForelegPoint.x) < actualForelegPoint.y ||
                ground->getInterpolatedWorldPoint(actualHindlegPoint.x) < actualHindlegPoint.y) && velocity.y > 0) {
                // This should rotate the dragon until both feet are on the ground, and only then change mode
                mode = Grounded;
                /*
                worldPosition.y =
                ground->getInterpolatedPoint(actualHindlegPoint.x) < ground->getInterpolatedPoint(actualForelegPoint.x) ?
                ground->getInterpolatedPoint(actualForelegPoint.x) + foreLeg.y : ground->getInterpolatedPoint(actualHindlegPoint.x) + hindLeg.y;
                */
                velocity.y = 0;
            }
            
            break;
        }
        case Grounded:
        {
            if(currentStamina < save.stats.stamina) {
                currentStamina += ((save.stats.stamina * (1 / staminaRegenTime)) / 60);
            } else {
                currentStamina = save.stats.stamina;
            }
            velocity.x *= landResistance;
            if(al_key_down(&input.keyboardState, ALLEGRO_KEY_D)) {
                velocity.x += save.stats.size * 2;
            }
            if(al_key_down(&input.keyboardState, ALLEGRO_KEY_A)) {
                velocity.x -= save.stats.size * 2;
            }
            if(shouldJump && currentStamina > flightCost)
            {
                velocity.y -= 100 * save.stats.wingspan;
                currentStamina -= flightCost;
                mode = Flying;
                jumpTime = al_get_time();
                currentSkin->jump.setTimeOffset();
            }
            worldPosition.x += (velocity.x / 60.);
            Point2D foreDesiredPoint(worldPosition.x + foreLeg.x, ground->getInterpolatedWorldPoint(foreLegWorldPos().x));
            Point2D hindDesiredPoint(worldPosition.x + hindLeg.x, ground->getInterpolatedWorldPoint(hindLegWorldPos().x));
            worldPosition.y = lerp(foreDesiredPoint, hindDesiredPoint, 0.5).y - foreLeg.y;
            currentRotation = angle(foreDesiredPoint - hindDesiredPoint, Point2D(100, 0));
            if(foreDesiredPoint.y < hindDesiredPoint.y)
            {
                currentRotation = -currentRotation;
            }
            currentDirection = velocity.x < 0 ? Left : Right;
            break;
        }
        
    }
    Point2D oldWorldPosition = worldPosition;
    worldPosition = ground->getLoopedCoordinate(worldPosition);
    //std::cerr << "World Position: " << worldPosition.x << '\n';
    if(oldWorldPosition != worldPosition) {
        return oldWorldPosition.x < worldPosition.x ? Right : Left;
        //std::cerr << "Screen flip! Time: " << al_get_time() << '\n';
    } else {
        return None;
    }
}

void Dragon::renderRotated(ALLEGRO_BITMAP* image, Point2D screenPos, int flags) const {
    Point2D drawPos = worldPosition - screenPos;
    al_draw_rotated_bitmap(image, al_get_bitmap_width(image) / 2, al_get_bitmap_height(image) / 2,
    drawPos.x, drawPos.y, currentRotation, flags);
    
}



void Dragon::renderStep(Point2D screenPos) const {
    int flags = 0;
    if(currentDirection == Left) {
        flags += ALLEGRO_FLIP_HORIZONTAL;
    }
    switch (mode) {
        case Flying :
        {
            if(al_get_time() - jumpTime < 0.5)
            {
                renderRotated(currentSkin->jump.getCurrentFrame(), screenPos, flags);
                break;
            }
            renderRotated(currentSkin->glide.getCurrentFrame(), screenPos, flags);
            break;
        }
        case Grounded :
        {
            if(velocity.x > 4 || velocity.x < -4) {
                renderRotated(currentSkin->run.getCurrentFrame(), screenPos, flags);
            } else {
                renderRotated(currentSkin->stand, screenPos, flags);
            }
            break;
        }
    }
    //Point2D drawPos = worldPosition - screenPos;
    //al_draw_filled_rectangle(drawPos.x - 20, drawPos.y - 15, drawPos.x + 20, drawPos.y + 15, al_map_rgb(50, 160, 170));
}

void Dragon::fireBreath(Point2D mainVelocity, double lifeTime, std::vector<Bullet> bulletList) const {
    mainVelocity.x *= (1 + (((double)(rand() % 1000) - 500) / 1000.));
    mainVelocity.y *= (1 + (((double)(rand() % 1000) - 500) / 1000.));
    bulletList.push_back(Bullet(new DragonFlame(worldPosition, mainVelocity, ground, 5)));
}
    
Point2D Dragon::foreLegWorldPos() const {
    return worldPosition + rotate(foreLeg, currentRotation);
}
Point2D Dragon::hindLegWorldPos() const {
    return worldPosition + rotate(hindLeg, currentRotation);
}
    
