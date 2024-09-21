#include <SFML/Graphics.hpp>
#include <vector>
#include <pthread.h>
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
};

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Asientos de Avión");

    // Crear asientos
    std::vector<std::vector<Asiento>> asientos(numFilas, std::vector<Asiento>(numColumnas));

    // Posicion de los asientos
    for (int fila = 0; fila < numFilas; ++fila) {
        for (int columna = 0; columna < numColumnas; ++columna) {
            asientos[fila][columna].forma.setSize(sf::Vector2f(anchoAsiento, altoAsiento));
            asientos[fila][columna].forma.setFillColor(sf::Color::Green); // Asiento disponible

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
