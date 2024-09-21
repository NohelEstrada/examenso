#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

using namespace std;

const int numFilas = 6;
const int numColumnas = 4;
const int anchoAsiento = 50;
const int altoAsiento = 50;
const int espacioAsiento = 10;
const int espacioPasillo = 50;

struct Asiento {
    sf::RectangleShape forma;
    bool reservado; // Para llevar el estado de reservación
};

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Asientos de Avión");

    // Crear asientos
    std::vector<std::vector<Asiento>> asientos(numFilas, std::vector<Asiento>(numColumnas));

    // Posición de los asientos
    for (int fila = 0; fila < numFilas; ++fila) {
        for (int columna = 0; columna < numColumnas; ++columna) {
            asientos[fila][columna].forma.setSize(sf::Vector2f(anchoAsiento, altoAsiento));
            asientos[fila][columna].forma.setFillColor(sf::Color::Green); // Asiento disponible
            asientos[fila][columna].reservado = false; // Inicialmente no reservado

            // Posición del asiento con un pasillo en el medio
            int desplazamientoX = (columna < 2) ? columna * (anchoAsiento + espacioAsiento) + 100 
                                                : columna * (anchoAsiento + espacioAsiento) + 100 + espacioPasillo;

            asientos[fila][columna].forma.setPosition(desplazamientoX, fila * (altoAsiento + espacioAsiento) + 50);
        }
    }

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();

            // Manejo de clic del mouse
            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                // Obtener la posición del clic
                sf::Vector2f posicionMouse = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));

                // Verificar si se hizo clic en un asiento
                for (int fila = 0; fila < numFilas; ++fila) {
                    for (int columna = 0; columna < numColumnas; ++columna) {
                        if (asientos[fila][columna].forma.getGlobalBounds().contains(posicionMouse)) {
                            if (!asientos[fila][columna].reservado) {
                                asientos[fila][columna].reservado = true; // Marcar como reservado
                                asientos[fila][columna].forma.setFillColor(sf::Color::Red); // Cambiar a rojo
                                std::cout << "Asiento [" << fila << ", " << columna << "] reservado." << std::endl;
                            } else {
                                std::cout << "El asiento [" << fila << ", " << columna << "] ya está reservado." << std::endl;
                            }
                        }
                    }
                }
            }
        }

        ventana.clear();

        // Dibujar los asientos
        for (int fila = 0; fila < numFilas; ++fila) {
            for (int columna = 0; columna < numColumnas; ++columna) {
                ventana.draw(asientos[fila][columna].forma);
            }
        }

        ventana.display();
    }

    return 0;
}
