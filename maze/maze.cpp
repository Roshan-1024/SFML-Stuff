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

    vector<vector<bool>> bitmask;

public:
    Game(){
        cout << "Welcome to the Maze game" << endl;
        if(!mazeBoardTexture.loadFromFile("assets/maze1.jpg")){
                cout << "maze image not found";
                exit(1);
        }
        mazeBoardSize = mazeBoardTexture.getSize();
        bitmask.assign(mazeBoardSize.y, vector<bool>(mazeBoardSize.x));

        _initBitmask();
    }

    void _initBitmask(){
        sf::Image image = mazeBoardTexture.copyToImage();
        const std::uint8_t* pixels = image.getPixelsPtr();
        sf::Vector2u size = image.getSize();

        for(int i = 0; i < size.y; i++){
            for(int j = 0; j < size.x; j++){
                int idx = (i * size.x + j) * 4; // skip by 4, cuz rgba per pixel
                uint8_t r = pixels[idx];
                uint8_t g = pixels[idx+1];
                uint8_t b = pixels[idx+2];
                // a not required

                if(r != 255 || g != 255 || b != 255){ // for black
                    bitmask[i][j] = true;
                }
            }
        }

    }

    void run(){
        sf::Sprite mazeBoardSprite(mazeBoardTexture);
        mazeBoardSprite.setScale(
            {(float)WIDTH/mazeBoardSize.x, (float)HEIGHT/mazeBoardSize.y}
        );
        // Game loop
        while(m_window.isOpen()){
            while(const std::optional<sf::Event> event = m_window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    m_window.close();
                }
            }

            m_window.clear(sf::Color::White);
            m_window.draw(mazeBoardSprite);
            m_window.display();
        }
    }
};

int main(){
    Game game;
    game.run();
}
