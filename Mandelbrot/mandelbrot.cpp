#include <SFML/Graphics.hpp>
#include <iostream>
#include <complex>

class Program {
private:
    const unsigned int width = 800;
    const unsigned int height = 600;
    const int maxIter = 200;

    sf::RenderWindow m_window{sf::VideoMode({width, height}), "Mandelbrot"};
    sf::CircleShape circle{50.f};
    sf::Image image;
    sf::Texture texture;

public:
    Program() {
        std::cout << "Hello and welcome to Mandelbrot generator" << std::endl;
        m_window.setVerticalSyncEnabled(true);
        circle.setFillColor(sf::Color::Blue);

        image.resize({width, height});

        for (unsigned int px = 0; px < width; ++px) {
            for (unsigned int py = 0; py < height; ++py) {

                // Map pixel to complex plane
                double x = (px - width/2.0) * 4.0 / width;
                double y = (py - height/2.0) * 4.0 / width;

                std::complex<double> c(x, y);
                std::complex<double> z(0, 0);

                int iter = 0;
                while (std::abs(z) <= 2.0 && iter < maxIter) {
                    z = z*z + c;
                    iter++;
                }

                if (iter == maxIter)
                    image.setPixel({px, py}, sf::Color::Black);
                else
                    image.setPixel({px, py}, sf::Color(255 * iter / maxIter, 0, 255));
            }
        }

        if (!texture.loadFromImage(image)) {
            std::cerr << "Failed to load texture from image!" << std::endl;
        }
    }

    void run() {
        sf::Sprite sprite(texture);
        while(m_window.isOpen()) {
            while(const std::optional<sf::Event> event = m_window.pollEvent()) {
                if(event->is<sf::Event::Closed>()) {
                    m_window.close();
                }
            }

            m_window.clear();
            m_window.draw(sprite);
            m_window.display();
        }
    }
};

int main() {
    Program program;
    program.run();
}
