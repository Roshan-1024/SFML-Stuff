#include <SFML/Graphics.hpp>
#include <iostream>
#include <vector>
using std::cout;
using std::endl;
using std::vector;


class Game{
private:
    const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    sf::RenderWindow m_window{sf::VideoMode({WIDTH, HEIGHT}), "Chess"};
    sf::Texture chessBoardTexture;
    sf::Texture whitePiecesTexture;
    sf::Texture blackPiecesTexture;
    sf::Vector2u chessBoardSize;
    sf::Vector2u piecesSize;

    bool DEBUG = false;

public:
    Game(){
        cout << "Welcome to my Chess game" << endl;
        if(!chessBoardTexture.loadFromFile("assets/board_light.png")){
            cout << "Chess board image not found!" << endl;
            exit(1);
        }
        chessBoardSize = chessBoardTexture.getSize();

        if(!whitePiecesTexture.loadFromFile("assets/WhitePieces.png")){
            cout << "White pieces texture not found!" << endl;
            exit(1);
        }
        if(!blackPiecesTexture.loadFromFile("assets/BlackPieces.png")){
            cout << "Black pieces texture not found!" << endl;
            exit(1);
        }
        piecesSize = whitePiecesTexture.getSize();
        piecesSize.x /= 6;

    }

    sf::Sprite _createPiece(sf::Texture& texture, int index){
        sf::Sprite piece(texture);
        piece.setTextureRect(
            sf::IntRect(
                { static_cast<int>(index * piecesSize.x), 0 },
                { static_cast<int>(piecesSize.x), static_cast<int>(piecesSize.y) }
            )
        );
        return piece;
    }

    void run(){
        //////////////////////////////////////////////
        ///                 BOARD SETUP
        //////////////////////////////////////////////
        sf::Sprite chessBoardSprite(chessBoardTexture);
        chessBoardSprite.setScale({(float)WIDTH / chessBoardSize.x, (float)HEIGHT / chessBoardSize.y});

        //////////////////////////////////////////////
        ///                 PIECES SETUP
        //////////////////////////////////////////////
        vector<sf::Sprite> whitePawnSprites;
        vector<sf::Sprite> whiteMainPiecesSprites;
        vector<sf::Sprite> blackMainPiecesSprites;
        vector<sf::Sprite> blackPawnSprites;
        vector<int> pieceTextureOrder = {2, 1, 3, 5, 4, 3, 1, 2}; // for main pieces
        float cellX = (float)WIDTH/8;
        float cellY = (float)HEIGHT/8 - 1.5;
        for(int i = 0; i < 8; i++){
            // white
            whitePawnSprites.emplace_back(_createPiece(whitePiecesTexture, 0));
            whiteMainPiecesSprites.emplace_back(_createPiece(whitePiecesTexture, pieceTextureOrder[i]));
            whitePawnSprites[i].setScale({
                    cellX/piecesSize.x,
                    cellY/piecesSize.y
            });
            whiteMainPiecesSprites[i].setScale({
                    cellX/piecesSize.x,
                    cellY/piecesSize.y
            });
            whitePawnSprites[i].setPosition({i * cellX, 6 * cellY});
            whiteMainPiecesSprites[i].setPosition({i * cellX, 7 * cellY});

            // black
            blackPawnSprites.emplace_back(_createPiece(blackPiecesTexture, 0));
            blackMainPiecesSprites.emplace_back(_createPiece(blackPiecesTexture, pieceTextureOrder[i]));
            blackPawnSprites[i].setScale({
                    cellX/piecesSize.x,
                    cellY/piecesSize.y
            });
            blackMainPiecesSprites[i].setScale({
                    cellX/piecesSize.x,
                    cellY/piecesSize.y
            });
            blackPawnSprites[i].setPosition({i * cellX, 1 * cellY});
            blackMainPiecesSprites[i].setPosition({i * cellX, 0 * cellY});
        }


        // Game loop
        while(m_window.isOpen()){
            while(const std::optional<sf::Event> event = m_window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    m_window.close();
                }
            }

            m_window.clear(sf::Color::White);
            m_window.draw(chessBoardSprite);
            for(int i = 0; i < 8; i++){
                m_window.draw(whitePawnSprites[i]);
                m_window.draw(whiteMainPiecesSprites[i]);
                m_window.draw(blackPawnSprites[i]);
                m_window.draw(blackMainPiecesSprites[i]);

                if(DEBUG){
                    // White pawn outline
                    {
                        auto bounds = whitePawnSprites[i].getGlobalBounds();
                        sf::RectangleShape outline({bounds.size.x, bounds.size.y});
                        outline.setPosition(bounds.position);
                        outline.setFillColor(sf::Color::Transparent);
                        outline.setOutlineColor(sf::Color::Red);
                        outline.setOutlineThickness(2.f);
                        m_window.draw(outline);
                    }

                    // Black pawn outline
                    {
                        auto bounds = blackPawnSprites[i].getGlobalBounds();
                        sf::RectangleShape outline({bounds.size.x, bounds.size.y});
                        outline.setPosition(bounds.position);
                        outline.setFillColor(sf::Color::Transparent);
                        outline.setOutlineColor(sf::Color::Blue);
                        outline.setOutlineThickness(2.f);
                        m_window.draw(outline);
                    }
                }
            }
            m_window.display();
        }
    }
};


int main(){
    Game game;
    game.run();
}
