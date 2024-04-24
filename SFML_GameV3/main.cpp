#include <SFML/Graphics.hpp>
#include "Player.h"
#include "Enemy.h"
#include "ShootingSystemV2.h"
#include "SpawnHandler.h"
#include <vector>

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));


    sf::RenderWindow window(sf::VideoMode(1000, 800), "SFML TopDownShooter v2");


    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("../grass.png")) {
        // Handle error loading texture
        return EXIT_FAILURE;
    }


    // Create background sprites in a 5x5 pattern
    std::vector<sf::Sprite> backgroundSprites;
    for (int i = 0; i < 100; ++i) {
        for (int j = 0; j < 100; ++j) {
            sf::Sprite sprite(backgroundTexture);
            sprite.setPosition(i * backgroundTexture.getSize().x, j * backgroundTexture.getSize().y);
            backgroundSprites.push_back(sprite);
        }
    }

    // Load player texture
    sf::Texture playerTexture;
    if (!playerTexture.loadFromFile("../Top_Down_Survivor/rifle/idle/survivor-idle_rifle_0.png")) {
        // Handle error loading texture
        return EXIT_FAILURE;
    }

    // Load enemy texture
    sf::Texture enemyTexture;
    if (!enemyTexture.loadFromFile("../export/skeleton-attack_0.png")) {
        // Handle error loading texture
        return EXIT_FAILURE;
    }


    Player player(playerTexture, 150.0f, 100, 360.0f);
    player.getSprite().setPosition(25600.0f,25600.0f);

    //player.getSprite().setPosition(window.getSize().x / 2, window.getSize().y / 2);
    player.getSprite().setScale(0.5f, 0.5f);


    std::vector<Enemy> enemies;


    // Create spawn handler
    SpawnHandler spawnHandler(enemies, enemyTexture, player, sf::seconds(1.0f));
    ShootingSystemV2 shootingSystemV2(player.getSprite(), 500.0, 700.0f);

    sf::Clock clock;
    sf::Time elapsedTime;



    // Main loop
    while (window.isOpen()) {
        sf::Time deltaTime = clock.restart();
        float dt = deltaTime.asSeconds();

        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }
        }


        // Handle shooting event
        if (sf::Mouse::isButtonPressed(sf::Mouse::Left)) {
            shootingSystemV2.shootBullet(window);
        }

        // Update entities
        player.update(window, dt);
        shootingSystemV2.updateBullets(dt);
        shootingSystemV2.checkBulletEnemyCollision(enemies); // Check collisions after updating bullets
        shootingSystemV2.updatePlayerAnimation(dt);


        // Update spawn handler
        spawnHandler.update(window,dt);
        spawnHandler.removeDeadEnemies();

        // Set the view center to the player's position
        sf::View view = window.getView();
        view.setCenter(player.getSprite().getPosition());
        window.setView(view);
        sf::Color grass (86,84,49);
        // Clear the window
        window.clear(grass);
        // Draw all background sprites
        for (const auto& sprite : backgroundSprites) {
            window.draw(sprite);
        }


        player.draw(window);
        spawnHandler.drawEnemies(window);

        //shootingSystemV2.drawLineToMouse(window);
        shootingSystemV2.drawBullets(window);

        window.display();
    }

    return EXIT_SUCCESS;
}

