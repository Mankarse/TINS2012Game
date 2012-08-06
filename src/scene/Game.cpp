#include "Game.h"
#include "Spawner.h"
#include "Hut.h"
template<typename T, std::size_t N>
static std::size_t length(T(&)[N]) {
    return N;
}

static GroundHeightmap loadGlobalHeightmap() {
    double points[] = {120., 110., 70., 79., 95., 190., 270., 270., 270., 270., 270., 270, 270., 270., 270, 260., 240.,
    100, 95, 90, 100, 190, 280, 285, 283, 280, 260, 230, 180, 110, 70, 75, 120, 170, 200, 140, 110, 110, 95, 105};
    
    return GroundHeightmap(100, static_cast<int>(length(points)), std::vector<double>(points, points + length(points)));
}

static Rect cavePosition() {
    return Rect(2250, 100, 200, 200);
}

static std::vector<Spawner> createSpawners() {
    std::vector<Spawner> spawners;
    spawners.push_back(Spawner(new Hut(Point2D(2500, 0))));
    return spawners;
}

static bool isDisplayClosedEvent(ALLEGRO_EVENT const& e) {
    return e.type == ALLEGRO_EVENT_DISPLAY_CLOSE;
}

Game::Game() :
    ground(loadGlobalHeightmap()),
    caveRect(cavePosition()),
    spawners(createSpawners()),
    player()
{
    player.assignHeightmap(&ground);
}

Game::Game(GameSave savedGame) :
    ground(loadGlobalHeightmap()),
    caveRect(cavePosition()),
    player(savedGame.stats)
{
    player.assignHeightmap(&ground);
}

void Game::init() {
    al_init_primitives_addon();
    screenCorner = Point2D(0, al_get_display_height(al_get_current_display()) * -0.8);
}

Point2D Game::mouseToWorldPos(Point2D const& mousePosition) const {
    Point2D retV(mousePosition + screenCorner);
    return retV;
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
    double layer = 0.6;
    double basePosition = ground.getTotalSize() / 2;
    double positionOffset = al_get_time() * 15; // Look at the clouds move! Happy now?
    Point2D screenspaceCentre (worldToScreenPoint(Point2D(basePosition + positionOffset, 300), layer));
    //std::cout << screenCorner.x << '\n';
    if(screenCorner.y - positionOffset < screenspaceCentre.y - (al_get_bitmap_height(image) * 0.5)) {
        screenspaceCentre.y -= al_get_bitmap_height(image);
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image)*4, al_get_bitmap_height(image),
            0);
        screenspaceCentre.y += al_get_bitmap_height(image);
    }
    al_draw_scaled_bitmap(
        image,
        0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
        screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
        al_get_bitmap_width(image), al_get_bitmap_height(image),
        0);
    if(screenCorner.x - positionOffset < 0)
    {
        screenspaceCentre = worldToScreenPoint(Point2D((-basePosition) + positionOffset, 300), layer);
        if(screenCorner.y < screenspaceCentre.y - (al_get_bitmap_height(image) * 0.5)) {
            screenspaceCentre.y -= al_get_bitmap_height(image);
            al_draw_scaled_bitmap(
                image,
                0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
                screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
                al_get_bitmap_width(image)*4, al_get_bitmap_height(image),
                0);
            screenspaceCentre.y += al_get_bitmap_height(image);
        }
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
    }
    if(screenCorner.x + al_get_display_width(al_get_current_display()) > ground.getTotalSize() / 2)
    {
        screenspaceCentre = worldToScreenPoint(Point2D((basePosition + ground.getTotalSize() + positionOffset), 300), layer);
        if(screenCorner.y < screenspaceCentre.y - (al_get_bitmap_height(image) * 0.5)) {
            screenspaceCentre.y -= al_get_bitmap_height(image);
            al_draw_scaled_bitmap(
                image,
                0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
                screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
                al_get_bitmap_width(image)*4, al_get_bitmap_height(image),
                0);
            screenspaceCentre.y += al_get_bitmap_height(image);
        }
        al_draw_scaled_bitmap(
            image,
            0, 0, al_get_bitmap_width(image), al_get_bitmap_height(image),
            screenspaceCentre.x - ((al_get_bitmap_width(image)*0.5)), screenspaceCentre.y - ((al_get_bitmap_height(image)*0.5)),
            al_get_bitmap_width(image), al_get_bitmap_height(image),
            0);
    }
}

Scene* Game::update(InputState const& input) {
    if (std::find_if(input.events.begin(), input.events.end(), isDisplayClosedEvent) != input.events.end()) {
        return 0;
    }
    for (std::vector<ALLEGRO_EVENT>::const_iterator it(input.events.begin()), end(input.events.end()); it != end; ++it)
    {
        ALLEGRO_EVENT const& event(*it);
        if (event.type == ALLEGRO_EVENT_KEY_DOWN && event.keyboard.keycode == ALLEGRO_KEY_ENTER) {
            GameSave save;
            save.stats = DragonStats(player.stats);
            save.totalPlayTime = 0;
            save.totalScore = 55;
            save.scoreDelta = 67;
            saveGame(save);
            std::auto_ptr<Cave> cave(new Cave(save));
            return cave.release();
        }
    }
    mousePosition.x = al_get_mouse_state_axis(&input.mouseState, 0);
    mousePosition.y = al_get_mouse_state_axis(&input.mouseState, 1);
    // spawners!
    
    for (std::vector<Spawner>::iterator it(spawners.begin()), end(spawners.end()); it != end; ++it)
    {
        Spawner& curSpawner(*it);
        std::vector<Enemy> newEnemies(curSpawner.update(totalPlayTime));
        enemies.insert(enemies.end(), newEnemies.begin(), newEnemies.end());
    }
    // Player!
    switch(player.physicsStep(input)) {
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
    screenCorner = ((mouseToWorldPos(mousePosition) + player.worldPosition) * 0.5) - Point2D(al_get_display_width(al_get_current_display()) * 0.5,
        al_get_display_height(al_get_current_display()) * 0.5);
    
    // Enemies!
    for(std::vector<Enemy>::iterator it(enemies.begin()), end(enemies.end()); it != end; ++it)
    {
        Enemy& curEnemy(*it);
        if(!curEnemy.hasHeightmap())
        {
            curEnemy.assignHeightmap(ground);
        }
        curEnemy.update(bullets, particles, player);
    }
    // My bullets!
    
    // Their bullets!
    
    // Particle effects update
    
    // Clean up dead things
    return this;
}

void Game::renderQueue(std::vector<Renderable const*> const& queue) const {
    for (std::vector<Renderable const*>::const_iterator it(queue.begin()), end(queue.end()); it != end; ++it)
    {
        Renderable const& curObject(**it);
        Point2D position(curObject.getWorldPoint());
        // renders the object 3 times!
        drawBitmapAtWorldPoint(curObject.getBitmap(), position, curObject.getDepth());
        drawBitmapAtWorldPoint(curObject.getBitmap(), Point2D(position.x + ground.getTotalSize(), position.y), curObject.getDepth());
        drawBitmapAtWorldPoint(curObject.getBitmap(), Point2D(position.x - ground.getTotalSize(), position.y), curObject.getDepth());
    }
}

void Game::preRender(RenderQueueSet* renderQueues) const {
    for (std::vector<StaticObject>::const_iterator it(staticRenderables.begin()), end(staticRenderables.end()); it != end; ++it)
    {
        StaticObject const& curObject(*it);
        curObject.pickRenderQueue(*renderQueues);
    }
    for (std::vector<Spawner>::const_iterator it(spawners.begin()), end(spawners.end()); it != end; ++it)
    {
        Spawner const& curSpawn(*it);
        curSpawn.pickRenderQueue(*renderQueues);
    }
    for (std::vector<Enemy>::const_iterator it(enemies.begin()), end(enemies.end()); it != end; ++it)
    {
        Enemy const& curEnemy(*it);
        curEnemy.pickRenderQueue(*renderQueues);
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
    al_draw_bitmap(g_GameUI, 0, 0, 0);
    al_draw_textf(g_Font10, al_map_rgb(0, 0, 0), 27*4, 12*4, 0, "%d", totalScore);
    al_draw_textf(g_Font10, al_map_rgb(0, 0, 0), 26*4, 21*4, 0, "%d", scoreDelta);
    int position(0);
    for (std::vector<unsigned>::reverse_iterator it(factors.rbegin()), end(factors.rend()); it != end; ++it) {
        int increment;
        al_draw_textf(g_Font10, al_map_rgb(0, 0, 0), 34*4 + position * 16, 30*4, 0, "%d%n", *it, &increment);
        position += increment + 1;
    }
}

void Game::drawingPass(RenderQueueSet* renderQueues) const {
    // Render each queue, in order
    // Background:
    drawBackground(g_LevelSky, 0.5);
    
    renderQueue(renderQueues->farBackground);
    renderQueue(renderQueues->nearBackground);
    // Heightmap
    drawBitmapAtWorldPoint(g_LevelFG, Point2D(0,0));
    drawBitmapAtWorldPoint(g_LevelFG, Point2D(ground.getTotalSize(),0));
    drawBitmapAtWorldPoint(g_LevelFG, Point2D(-ground.getTotalSize(),0));
    Point2D worldTopCorner(-ground.getTotalSize(), 300);
    worldTopCorner = worldToScreenPoint(worldTopCorner);
    Point2D worldBottomCorner(ground.getTotalSize() * 2, 1000);
    worldBottomCorner = worldToScreenPoint(worldBottomCorner);
    al_draw_filled_rectangle(worldTopCorner.x, worldTopCorner.y, worldBottomCorner.x, worldBottomCorner.y, al_map_rgb(146, 120, 94));
    renderQueue(renderQueues->middleGround);
    player.renderStep(screenCorner);
    
    drawCave();
    
    //ground.draw(screenCorner);
    renderQueue(renderQueues->foreground);
    
    drawUI(30,24,2,2,2,2,2,2);
}

void Game::renderTo(ALLEGRO_BITMAP* target) const {
    // Collect renderables, add to queues
    // Background objects
    RenderQueueSet renderQueues;
    preRender(&renderQueues);
    
    // Now, render the collected queues
    drawingPass(&renderQueues);

}

