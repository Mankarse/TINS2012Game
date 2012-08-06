#include "Game.h"
#include "Spawner.h"
#include "Hut.h"
#include "StaticObject.h"
#include "Bitmap.h"
#include "MainMenu.h"
template<typename T, std::size_t N>
static std::size_t length(T(&)[N]) {
    return N;
}

static GroundHeightmap loadGlobalHeightmap() {
    double points[] = {
        120., 110., 70.,  79.,  95., 190., 270., 270., 270., 270., 270., 270,
        270., 270., 270, 260., 240.,  100,   95,   90,  100,  190,  280, 285,
         283, 284,  284,  283,  284,  284,  284,  283,  282,  284,  260, 230,
         180, 110,   70,   75,  120,  170,  200,  140,  110,  110,   95, 105,};
    
    return GroundHeightmap(100, static_cast<int>(length(points)), std::vector<double>(points, points + length(points)));
}

static Rect cavePosition() {
    return Rect(2700, 100, 200, 200);
}

static std::vector<Spawner> createSpawners() {
    std::vector<Spawner> spawners;
    spawners.push_back(Spawner(new Hut(Point2D(700 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(1188 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(1632 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(2188 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(2636 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(3988 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(4120 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(4292 - 2400, 0))));
    spawners.push_back(Spawner(new Hut(Point2D(4640 - 2400, 0))));
    return spawners;
}

static std::vector<StaticObject> createStaticObjects() {
    std::vector<StaticObject> staticObjs;
    staticObjs.push_back(StaticObject(g_Bitmaps["LevelFG"], Point2D(), 1, Middle));
    staticObjs.push_back(StaticObject(g_Bitmaps["Mountains"], Point2D(0, 300), 0.6, FarBG));
    
    staticObjs.push_back(StaticObject(g_Bitmaps["Cave"], Point2D(2800, 100), 0.95, NearBG));
    return staticObjs;
}

static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

Game::Game() :
    gamePaused(),
    ground(loadGlobalHeightmap()),
    caveRect(cavePosition()),
    spawners(createSpawners()),
    staticRenderables(createStaticObjects()),
    player()
{
    player.assignHeightmap(&ground);
    player.worldPosition = Point2D(caveRect.x + caveRect.width / 2, caveRect.y + caveRect.height * 0.7);
}

Game::Game(GameSave savedGame) :
    gamePaused(),
    ground(loadGlobalHeightmap()),
    caveRect(cavePosition()),
    spawners(createSpawners()),
    staticRenderables(createStaticObjects()),
    player(savedGame)
{
    player.assignHeightmap(&ground);
    player.worldPosition = Point2D(caveRect.x + caveRect.width / 2, caveRect.y + caveRect.height * 0.7);
}

void Game::init() {
    al_init_primitives_addon();
    al_play_sample(g_Samples["GameMus"], 0.4, 0, 1, ALLEGRO_PLAYMODE_LOOP, &music);
}

Point2D Game::mouseToWorldPos(Point2D const& mousePosition) const {
    return Point2D(mousePosition + screenCorner);
}

Point2D Game::worldToScreenPoint(Point2D const& worldPosition, double layer) const {
    return (worldPosition - screenCorner) * layer;
}

void Game::drawBitmapAtWorldPoint(ALLEGRO_BITMAP* image, Point2D point, double layer) const {
    Point2D screenPos = worldToScreenPoint(point, layer);
    al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenPos.x - ((al_get_bitmap_width(image)*0.5) * layer), screenPos.y - ((al_get_bitmap_height(image)*0.5) * layer),
        al_get_bitmap_width(image) * layer, al_get_bitmap_height(image) * layer,
        0);
}
void Game::drawBitmapAtScreenPoint(ALLEGRO_BITMAP* image, Point2D point) const {
    al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        point.x - ((al_get_bitmap_width(image)*0.5)), point.y - ((al_get_bitmap_height(image)*0.5)),
        al_get_bitmap_width(image), al_get_bitmap_height(image),
        0);
}
void Game::drawBackground(ALLEGRO_BITMAP* image, double depth) const {
    double layer = 0.5;
    double basePosition = ground.getTotalSize() / 2;
    double positionOffset = player.save.totalPlayTime * 15; // Look at the clouds move! Happy now?
    while(positionOffset > basePosition) {
        positionOffset -= basePosition * 2;
    }
    double worldHeight = 0;
    while(worldHeight - al_get_bitmap_height(image) > screenCorner.y) {
        worldHeight -= al_get_bitmap_height(image) * 2;
    }
    Point2D screenspaceCentre (worldToScreenPoint(Point2D(basePosition + positionOffset, worldHeight), layer));
    //std::cout << screenCorner.x << '\n';
    al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
        al_get_bitmap_width(image), al_get_bitmap_height(image),
        0);
    al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y + ((al_get_bitmap_height(image)*0.5)),
        al_get_bitmap_width(image), al_get_bitmap_height(image),
        0);
    if(screenCorner.x - positionOffset < 0)
    {
        screenspaceCentre = worldToScreenPoint(Point2D((-basePosition) + positionOffset, worldHeight), layer);
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y + ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
    }
    if(screenCorner.x + al_get_display_width(al_get_current_display()) > ground.getTotalSize() / 2)
    {
        screenspaceCentre = worldToScreenPoint(Point2D((basePosition + ground.getTotalSize() + positionOffset), worldHeight), layer);
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y + ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
    }
}

bool shouldRemove(Enemy const& check) {
    return check.shouldDie();
}

Scene* Game::update(InputState const& input) {
    if (std::find_if(input.events.begin(), input.events.end(), isDisplayClosedEvent) != input.events.end()) {
        return 0;
    }
    for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
    {
        ALLEGRO_EVENT const& event(*it);
        if (gamePaused) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN
             && event.keyboard.keycode == ALLEGRO_KEY_R)
            {
                gamePaused = false;
                continue;
            }
            if (event.type == ALLEGRO_EVENT_KEY_DOWN
             && event.keyboard.keycode == ALLEGRO_KEY_Q)
            {
                std::auto_ptr<MainMenu> mainMenu(new MainMenu());
                al_stop_sample(&music);
                mainMenu->init();
                return mainMenu.release();
            }
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            saveGame(player.save);
            std::auto_ptr<Cave> cave(new Cave(player.save));
            al_stop_sample(&music);
            return cave.release();
        }
        if (event.type == ALLEGRO_EVENT_KEY_DOWN
            && (event.keyboard.keycode == ALLEGRO_KEY_ESCAPE || event.keyboard.keycode == ALLEGRO_KEY_P))
        {
            gamePaused = true;
            return this;
        }
    }
    
    if (gamePaused) {
        return this;
    }
    mousePosition.x = al_get_mouse_state_axis(&input.mouseState, 0);
    mousePosition.y = al_get_mouse_state_axis(&input.mouseState, 1);
    player.save.totalPlayTime += 1. / 60.;
    //std::cout << "Play Time: " << totalPlayTime << '\n';
    // spawners!
    
    for (std::vector<Spawner>::iterator it(spawners.begin()), end(spawners.end()); it != end; ++it)
    {
        Spawner& curSpawner(*it);
        std::vector<Enemy> newEnemies(curSpawner.update(player.save.totalPlayTime));
        enemies.insert(enemies.end(), newEnemies.begin(), newEnemies.end());
    }
    // Player!
    switch(player.physicsStep(input, screenCorner, friendlyBullets)) {
        case Left :
        {
            screenCorner.x -= ground.getTotalSize();
            break;
        }
        case Right :
        {
            screenCorner.x += ground.getTotalSize();
            break;
        }
        case None :
        {
            break;
        }
    }

    // Camera control
    screenCorner =
            ((mouseToWorldPos(mousePosition) + player.worldPosition) * 0.5)
            - Point2D(
                al_get_display_width(al_get_current_display()) * 0.5,
                al_get_display_height(al_get_current_display()) * 0.5);
    
    // Enemies!
    for(std::vector<Enemy>::iterator it(enemies.begin()), end(enemies.end()); it != end; ++it)
    {
        Enemy& curEnemy(*it);
        if(!curEnemy.hasHeightmap())
        {
            curEnemy.assignHeightmap(ground);
        }
        curEnemy.update(enemyBullets, particles, player);
    }
    // My bullets!
    for (std::vector<Bullet>::iterator it(friendlyBullets.begin()), end(friendlyBullets.end()); it != end; ++it)
    {
        Bullet& curBullet(*it);
        curBullet.update(enemies);
    }
    // Their bullets!
    for (std::vector<Bullet>::iterator it(enemyBullets.begin()), end(enemyBullets.end()); it != end; ++it)
    {
        Bullet& curBullet(*it);
        curBullet.update(player);
    }
    // Particle effects update
    
    // Clean up dead things
    enemies.erase(std::remove_if(enemies.begin(), enemies.end(), shouldRemove), enemies.end());
    return this;
}

void Game::drawWorldObject(ALLEGRO_BITMAP* bitmap, Point2D const& position, double depth) const {
    // renders the object 3 times!
    drawBitmapAtWorldPoint(bitmap, position, depth);
    drawBitmapAtWorldPoint(bitmap, Point2D(position.x + ground.getTotalSize(), position.y), depth);
    drawBitmapAtWorldPoint(bitmap, Point2D(position.x - ground.getTotalSize(), position.y), depth);
}

void Game::renderQueue(std::vector<Renderable const*> const& queue) const {
    for (std::vector<Renderable const*>::const_iterator it(queue.begin()), end(queue.end()); it != end; ++it)
    {
        Renderable const& curObject(**it);
        drawWorldObject(curObject.getBitmap(), curObject.getWorldPoint(), curObject.getDepth());
    }
}

void Game::preRender(RenderQueueSet& renderQueues) const {
    for (std::vector<StaticObject>::const_iterator it(staticRenderables.begin()), end(staticRenderables.end()); it != end; ++it)
    {
        StaticObject const& curObject(*it);
        curObject.pickRenderQueue(renderQueues);
    }
    for (std::vector<Spawner>::const_iterator it(spawners.begin()), end(spawners.end()); it != end; ++it)
    {
        Spawner const& curSpawn(*it);
        curSpawn.pickRenderQueue(renderQueues);
    }
    for (std::vector<Enemy>::const_iterator it(enemies.begin()), end(enemies.end()); it != end; ++it)
    {
        Enemy const& curEnemy(*it);
        curEnemy.pickRenderQueue(renderQueues);
    }
    for (std::vector<Bullet>::const_iterator it(friendlyBullets.begin()), end(friendlyBullets.end()); it != end; ++it)
    {
        Bullet const& curBullet(*it);
        curBullet.pickRenderQueue(renderQueues);
    }
    for (std::vector<Bullet>::const_iterator it(enemyBullets.begin()), end(enemyBullets.end()); it != end; ++it)
    {
        Bullet const& curBullet(*it);
        curBullet.pickRenderQueue(renderQueues);
    }

}

void Game::drawCave() const {
    Point2D caveTopCorner(worldToScreenPoint(Point2D(caveRect.x, caveRect.y)));
    Point2D caveBottomCorner(worldToScreenPoint(Point2D(caveRect.maxX(), caveRect.maxY())));
    al_draw_line(caveTopCorner.x, caveTopCorner.y, caveTopCorner.x, caveBottomCorner.y, al_map_rgb(255, 0, 0), 3);
    al_draw_line(caveTopCorner.x, caveTopCorner.y, caveBottomCorner.x, caveTopCorner.y, al_map_rgb(255, 0, 0), 3);
    al_draw_line(caveBottomCorner.x, caveTopCorner.y, caveBottomCorner.x, caveBottomCorner.y, al_map_rgb(255, 0, 0), 3);
    al_draw_line(caveBottomCorner.x, caveBottomCorner.y, caveTopCorner.x, caveBottomCorner.y, al_map_rgb(255, 0, 0), 3);
}

static void drawUI(
    unsigned totalScore,
    unsigned scoreDelta,
    double health, double maxHealth,
    double stamina, double maxStamina,
    double flames, double maxFlames)
{
    std::vector<unsigned> factors(factorize(scoreDelta));
    al_draw_bitmap(g_Bitmaps["GameUI"], 0, 0, 0);
    al_draw_textf(g_Font10, al_map_rgb(0, 0, 0), 27*4, 12*4, 0, "%d", totalScore);
    al_draw_textf(g_Font10, al_map_rgb(0, 0, 0), 26*4, 21*4, 0, "%d", scoreDelta);
    int position(0);
    for (std::vector<unsigned>::reverse_iterator it(factors.rbegin()), end(factors.rend()); it != end; ++it) {
        int increment;
        //Here we assume that all characters are 16 pixels wide...
        //This is clearly false, but fixing it means working out the details
        //of the variable width font.
        char const* formatStr = "%d%n";
        al_draw_textf(g_Font10, al_map_rgb(0, 0, 0), 34*4 + position * 16, 30*4, 0, formatStr, *it, &increment);
        position += increment + 1;
    }
    {
        double x = 266;
        double y = 500;
        al_draw_bitmap(g_Bitmaps["HealthEmpty"], x, y, 0);
        al_draw_bitmap_region(g_Bitmaps["HealthFull"], 0, 0, health/maxHealth * al_get_bitmap_width(g_Bitmaps["HealthFull"]), al_get_bitmap_height(g_Bitmaps["HealthFull"]), x, y, 0);
    }
    {
        double x = 10;
        double y = 540;
        al_draw_bitmap(g_Bitmaps["StaminaEmpty"], x, y, 0);
        al_draw_bitmap_region(g_Bitmaps["StaminaFull"], 0, 0, stamina/maxStamina * al_get_bitmap_width(g_Bitmaps["StaminaFull"]), al_get_bitmap_height(g_Bitmaps["StaminaFull"]), x, y, 0);
    }
    {
        double x = 130*4;
        double y = 540;
        al_draw_bitmap(g_Bitmaps["FlameCDEmpty"], x, y, 0);
        al_draw_bitmap_region(g_Bitmaps["FlameCDFull"], 0, 0, flames/maxFlames * al_get_bitmap_width(g_Bitmaps["FlameCDFull"]), al_get_bitmap_height(g_Bitmaps["FlameCDFull"]), x, y, 0);
    }
}

void Game::drawingPass(RenderQueueSet& renderQueues) const {
    // Render each queue, in order
    // Sky:
    drawBackground(g_Bitmaps["LevelSky"], 0.5);
    
    renderQueue(renderQueues.farBackground);
    
    renderQueue(renderQueues.midBackground);
    renderQueue(renderQueues.nearBackground);
    // Underground:
    Point2D worldTopCorner(-ground.getTotalSize(), 290);
    worldTopCorner = worldToScreenPoint(worldTopCorner);
    Point2D worldBottomCorner(ground.getTotalSize() * 2, 1000);
    worldBottomCorner = worldToScreenPoint(worldBottomCorner);
    al_draw_filled_rectangle(worldTopCorner.x, worldTopCorner.y, worldBottomCorner.x, worldBottomCorner.y, al_map_rgb(146, 120, 94));
    
    renderQueue(renderQueues.middleGround);
    player.renderStep(screenCorner);
    
    //drawCave();
    
    //ground.draw(screenCorner);
    renderQueue(renderQueues.foreground);
    
    drawUI(player.save.totalScore,player.save.scoreDelta,player.currentHealth,player.save.stats.size,player.currentStamina,player.save.stats.stamina,player.currentCooldown,player.save.stats.fireCooldown);
    
    if (gamePaused) {
        al_draw_bitmap(g_Bitmaps["Pause"], 0, 0, 0);
    }
}

void Game::renderTo(ALLEGRO_BITMAP* target) const {
    // Collect renderables, add to queues
    // Background objects
    RenderQueueSet renderQueues;
    preRender(renderQueues);
    
    // Now, render the collected queues
    drawingPass(renderQueues);

}

