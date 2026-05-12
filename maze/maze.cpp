#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;
using std::uint8_t;


class Game{
private:
    static constexpr unsigned int WIDTH = 800;
    static constexpr unsigned int HEIGHT = 600;
    sf::RenderWindow m_window{sf::VideoMode({WIDTH, HEIGHT}), "Maze"};
    sf::Vector2u mazeBoardSize;
    sf::Texture mazeBoardTexture;
    vector<vector<bool>> bitmask; // 0: obstacle, 1: access
    sf::Texture overlayTexture;
    float speed = 3.f;

    bool DEBUG = false;
    bool changed = true;

public:
    Game(){
        cout << "Welcome to the Maze game" << endl;
        m_window.setFramerateLimit(30);
        if(!mazeBoardTexture.loadFromFile("assets/maze1.jpg")){
            cout << "maze image not found";
            exit(1);
        }
        mazeBoardSize = mazeBoardTexture.getSize();
        bitmask.assign(mazeBoardSize.y, vector<bool>(mazeBoardSize.x, true));

        _initBitmask();
        if(DEBUG)
            _createOverlay();
    }

    void _initBitmask(){
        sf::Image image = mazeBoardTexture.copyToImage();
        const std::uint8_t* pixels = image.getPixelsPtr();

        for(int i = 0; i < mazeBoardSize.y; i++){
            for(int j = 0; j < mazeBoardSize.x; j++){
                int idx = (i * mazeBoardSize.x + j) * 4; // skip by 4, cuz rgba per pixel
                uint8_t r = pixels[idx];
                uint8_t g = pixels[idx+1];
                uint8_t b = pixels[idx+2];
                // a not required

                if(r == 0 && g == 0 && b == 0){ // for black
                    bitmask[i][j] = false;
                }
            }
        }
    }

    void _createOverlay(){
        sf::Image overlay({mazeBoardSize.x, mazeBoardSize.y}, sf::Color(0, 0, 0, 0));
        for(unsigned int y = 0; y < mazeBoardSize.y; y++){
            for(unsigned int x = 0; x < mazeBoardSize.x; x++){
                if(!bitmask[y][x]){
                    overlay.setPixel({x, y}, sf::Color(255, 0, 0, 255));
                }
            }
        }

        if(!overlayTexture.loadFromImage(overlay)){
            cout << "overlay loading from image failed";
            exit(1);
        }
    }

    bool canTread(sf::Vector2f& pos){
        float playerX = pos.x;
        float playerY = pos.y;

        float scaleX = (float)mazeBoardSize.x / WIDTH;
        float scaleY = (float)mazeBoardSize.y / HEIGHT;

        unsigned int pixelX = scaleX * playerX;
        unsigned int pixelY = scaleY * playerY;


        if(!bitmask[pixelY][pixelX]){
            return false;
        }
        return true;
    }

    void _handleMovements(sf::CircleShape& player){
        sf::Vector2f currPos = player.getPosition();
        sf::Vector2f newPos = currPos;
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::K) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Up)){
            newPos = {currPos.x, currPos.y-speed};
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::L) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Right)){
            newPos = {currPos.x+speed, currPos.y};
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::J) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Down)){
            newPos = {currPos.x, currPos.y+speed};
        }
        if(sf::Keyboard::isKeyPressed(sf::Keyboard::Key::H) || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Left)){
            newPos = {currPos.x-speed, currPos.y};
        }

        if(canTread(newPos)){
            player.setPosition(newPos);
        }
    }

    void run(){
        sf::Sprite mazeBoardSprite(mazeBoardTexture);
        mazeBoardSprite.setScale(
            {(float)WIDTH/mazeBoardSize.x, (float)HEIGHT/mazeBoardSize.y}
        );
        sf::Sprite overlaySprite(overlayTexture);
        overlaySprite.setScale(
            {(float)WIDTH/mazeBoardSize.x, (float)HEIGHT/mazeBoardSize.y}
        );

        // player
        float radius = 5.f;
        sf::CircleShape player(radius);
        player.setFillColor(sf::Color(255, 0, 0));

        // Game loop
        while(m_window.isOpen()){
            while(const std::optional<sf::Event> event = m_window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    m_window.close();
                }
                if(sf::Mouse::isButtonPressed(sf::Mouse::Button::Left)){
                    sf::Vector2f pos = m_window.mapPixelToCoords(sf::Mouse::getPosition(m_window));
                    player.setPosition({pos.x - radius, pos.y - radius});
                    if(DEBUG)
                        cout << "[click] " << pos.x << ", " << pos.y << endl;
                }
            }
            _handleMovements(player);

            m_window.clear(sf::Color::White);
            m_window.draw(mazeBoardSprite);
            m_window.draw(player);
            if(DEBUG)
                m_window.draw(overlaySprite);
            m_window.display();
        }
    }
};

int main(){
    Game game;
    game.run();
}
