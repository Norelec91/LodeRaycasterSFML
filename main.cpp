#include <vector>

#include <SFML/Graphics.hpp>

int main()
{
    const int screenWidth = 640;
    const int screenHeight = 480;
    const int mapWidth = 24;
    const int mapHeight = 24;
    
    int worldMap[mapWidth][mapHeight] =
    {
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,2,2,2,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
      {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,3,0,0,0,3,0,0,0,1},
      {1,0,0,0,0,0,2,0,0,0,2,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,2,2,0,2,2,0,0,0,0,3,0,3,0,3,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,0,0,0,0,5,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,0,4,0,0,0,0,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,0,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,4,4,4,4,4,4,4,4,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
      {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
    };

    double playerX = 22;
    double playerY = 12;
    
    double dirX = -1;
    double dirY = 0; // initial direction vector

    double planeX = 0;
    double planeY = 0.66; // the 2D raycaster version of camera plane

    int** buffer = new int* [screenHeight];

    for (int i = 0; i < screenHeight; i++)
    {
        buffer[i] = new int[screenWidth];
    }
    
    std::vector<int> texture[8];

    sf::RenderWindow window(sf::VideoMode(screenWidth, screenHeight), "Raycaster");

    sf::Clock deltaClock;

    while (window.isOpen())
    {
        sf::Time deltaTime = deltaClock.restart();

        window.clear();

        for (int x = 0; x < screenWidth; x++)
        {
            double cameraX = 2 * x / (double)screenWidth - 1; // x-coordinate in camera space
            double rayDirX = dirX + planeX * cameraX;
            double rayDirY = dirY + planeY * cameraX;

            int mapX = int(playerX);
            int mapY = int(playerY);

            double sideDistX;
            double sideDistY;

            double deltaDistX = (rayDirX == 0) ? 1e30 : std::abs(1 / rayDirX);
            double deltaDistY = (rayDirY == 0) ? 1e30 : std::abs(1 / rayDirY);

            double perpWallDist;

            int stepX;
            int stepY;

            int hit = 0; // was there a wall hit?
            int side; // was a North-South or a East-West wall hit?

            if (rayDirX < 0)
            {
                stepX = -1;
                sideDistX = (playerX - mapX) * deltaDistX;
            }
            else
            {
                stepX = 1;
                sideDistX = (mapX + 1.0 - playerX) * deltaDistX;
            }
            if (rayDirY < 0)
            {
                stepY = -1;
                sideDistY = (playerY - mapY) * deltaDistY;
            }
            else
            {
                stepY = 1;
                sideDistY = (mapY + 1.0 - playerY) * deltaDistY;
            }
            
            // perform DDA
            while (hit == 0)
            {
                // jump to next map square, either in x-direction, or in y-direction
                if (sideDistX < sideDistY)
                {
                    sideDistX += deltaDistX;
                    mapX += stepX;
                    side = 0;
                }
                else
                {
                    sideDistY += deltaDistY;
                    mapY += stepY;
                    side = 1;
                }
                
                // check if ray has hit a wall
                if (worldMap[mapX][mapY] > 0) {
                    hit = 1;
                }
            }

            // Calculate distance projected on camera direction. This is the shortest distance from the point where the wall is
            // hit to the camera plane. Euclidean to center camera point would give fisheye effect!
            // This can be computed as (mapX - posX + (1 - stepX) / 2) / rayDirX for side == 0, or same formula with Y
            // for size == 1, but can be simplified to the code below thanks to how sideDist and deltaDist are computed:
            // because they were left scaled to |rayDir|. sideDist is the entire length of the ray above after the multiple
            // steps, but we subtract deltaDist once because one step more into the wall was taken above.
            if (side == 0)
            {
                perpWallDist = (sideDistX - deltaDistX);
            }
            else
            {
                perpWallDist = (sideDistY - deltaDistY);
            }

            // calculate height of line to draw on screen
            int lineHeight = (int)(screenHeight / perpWallDist);

            // calculate lowest and highest pixel to fill in current stripe
            int drawStart = -lineHeight / 2 + screenHeight / 2;

            if (drawStart < 0)
            {
                drawStart = 0;
            }
            
            int drawEnd = lineHeight / 2 + screenHeight / 2;

            if (drawEnd >= screenHeight)
            {
                drawEnd = screenHeight - 1;
            }

            // choose wall color
            sf::Color color;

            switch (worldMap[mapX][mapY])
            {
                case 1:  color = sf::Color::Red;    break; // red
                case 2:  color = sf::Color::Green;  break; // green
                case 3:  color = sf::Color::Blue;   break; // blue
                case 4:  color = sf::Color::White;  break; // white
                default: color = sf::Color::Yellow; break; // yellow
            }

            if (side == 1)
            {
                color.r = color.r / 2;
                color.g = color.g / 2;
                color.b = color.b / 2;
            }

            sf::Vertex line[] =
            {
                sf::Vertex(sf::Vector2f(x, drawStart), color),
                sf::Vertex(sf::Vector2f(x, drawEnd), color)
            };

            window.draw(line, 2, sf::Lines);
        }

        float dt = deltaTime.asSeconds();
        double moveSpeed = dt * 100.0; // the constant value is in squares/second
        double rotSpeed = dt * 10.0; // the constant value is in radians/second

        sf::Event event;

        while (window.pollEvent(event))
        {
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
            {
                // move forward if no wall in front of you
                if (worldMap[int(playerX + dirX * moveSpeed)][int(playerY)] == false)
                {
                    playerX += dirX * moveSpeed;
                }

                if (worldMap[int(playerX)][int(playerY + dirY * moveSpeed)] == false)
                {
                    playerY += dirY * moveSpeed;
                }
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
            {
                if (worldMap[int(playerX - dirX * moveSpeed)][int(playerY)] == false)
                {
                    playerX -= dirX * moveSpeed;
                }

                if (worldMap[int(playerX)][int(playerY - dirY * moveSpeed)] == false)
                {
                    playerY -= dirY * moveSpeed;
                }
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
            {
                if (worldMap[int(playerX - dirX * moveSpeed)][int(playerY)] == false)
                {
                    // both camera direction and camera plane must be rotated
                    double oldDirX = dirX;
                    dirX = dirX * cos(rotSpeed) - dirY * sin(rotSpeed);
                    dirY = oldDirX * sin(rotSpeed) + dirY * cos(rotSpeed);

                    double oldPlaneX = planeX;
                    planeX = planeX * cos(rotSpeed) - planeY * sin(rotSpeed);
                    planeY = oldPlaneX * sin(rotSpeed) + planeY * cos(rotSpeed);
                }
            }

            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
            {
                if (worldMap[int(playerX - dirX * moveSpeed)][int(playerY)] == false)
                {
                    // both camera direction and camera plane must be rotated
                    double oldDirX = dirX;
                    dirX = dirX * cos(-rotSpeed) - dirY * sin(-rotSpeed);
                    dirY = oldDirX * sin(-rotSpeed) + dirY * cos(-rotSpeed);

                    double oldPlaneX = planeX;
                    planeX = planeX * cos(-rotSpeed) - planeY * sin(-rotSpeed);
                    planeY = oldPlaneX * sin(-rotSpeed) + planeY * cos(-rotSpeed);
                }
            }
            
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Escape))
            {
                window.close();
            }
        }

        window.display();
    }

    return 0;
}