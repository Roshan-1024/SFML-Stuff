#include <SFML/Graphics.hpp>
#include <iostream>
#include <cmath>
#include <numbers>
#include <random>
#include <vector>
using std::cout;
using std::endl;
using std::cos;
using std::sin;
using std::numbers::pi;
using std::vector;
using std::max;

struct BallProperties{
	sf::Vector2f init_pos;
    float angle;
	float radius;
    sf::Color fillColor;
    float outlineThickness;
    sf::Color outlineColor;
    float speed;
};

class Ball : public sf::CircleShape{
private:
    sf::Vector2f velocity;
public:
	Ball(const BallProperties& properties)
        : sf::CircleShape(properties.radius),
        velocity({properties.speed*cos(properties.angle), properties.speed*sin(properties.angle)}){

        // Applying the properties to the ball
        setPosition(properties.init_pos);
        setFillColor(properties.fillColor);
        setOutlineThickness(properties.outlineThickness);
        setOutlineColor(properties.outlineColor);
    }

    sf::Vector2f getVelocity() const{
        return velocity;
    }

    void updatePosition(){
        sf::Vector2f pos = getPosition();
        float r = getRadius();
        if(pos.x <= 0 || pos.x + 2*r >= 800)  this->velocity.x *= -1;
        if(pos.y <= 0 || pos.y + 2*r >= 600) this->velocity.y *= -1;

        sf::Vector2f new_position(pos.x + velocity.x, pos.y - velocity.y);
        setPosition(new_position);
    }

};

class Game{
private:
	const unsigned int WIDTH = 800;
    const unsigned int HEIGHT = 600;
    sf::RenderWindow m_window{sf::VideoMode({WIDTH, HEIGHT}), "Bouncy Ball"};
    vector<Ball>& balls;
    sf::Texture backgroundTexture;
    sf::Vector2u textureSize;


public:
	Game(vector<Ball>& balls) : balls(balls){
        cout << "Hello and welcome to bouncy ball simulator" << endl;
        m_window.setVerticalSyncEnabled(true);
        if(!backgroundTexture.loadFromFile("background.png")){
            cout << "Background image could not be loaded";
            exit(1);
        }
        textureSize = backgroundTexture.getSize();
	}

    void run(){
        sf::Sprite bgSprite(backgroundTexture);
        bgSprite.setScale({(float)WIDTH / textureSize.x, (float)HEIGHT / textureSize.y});
        while(m_window.isOpen()){
            while(const std::optional<sf::Event> event = m_window.pollEvent()){
                if(event->is<sf::Event::Closed>()){
                    m_window.close();
                }
            }

            for(auto& ball : balls)
                ball.updatePosition();

            m_window.clear(sf::Color::White);
            m_window.draw(bgSprite);
            for(auto& ball : balls)
                m_window.draw(ball);
            m_window.display();
        }
    }
};

int main(){
    float radius = 25.f;
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> distX(radius, 800.f - 2*radius);
    std::uniform_real_distribution<float> distY(radius, 600.f - 2*radius);
    std::uniform_real_distribution<float> randAngle(0.f, 2*pi);
    std::uniform_real_distribution<float> speed(1.f, 15.f);

    sf::Color c1(70, 140, 220), c2(20, 55, 110);
    vector<Ball> balls;
    for(int i = 0; i < 10; i++){
        BallProperties bp{
            {distX(gen), distY(gen)},
            randAngle(gen),
            radius,
            c1,
            5.f,
            c2,
            speed(gen)
        };
        balls.emplace_back(bp);
    }
    Game game(balls);
    game.run();
}
