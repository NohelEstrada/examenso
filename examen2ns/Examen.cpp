#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>
#include <pthread.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int filas = 6;
const int columnas = 4;
const int ancho = 50;
const int alto = 50;
const int espacio = 10;
const int pasillo = 50;

struct Silla {
    sf::RectangleShape rect;
    bool ocupado; // Para saber si está reservado
};

// Vector global para los asientos
std::vector<std::vector<Silla>> asientos(filas, std::vector<Silla>(columnas));

// Función para que cada hilo reserve un asiento
void* reservar(void* usuarioPtr) {
    int usuario = *(int*)usuarioPtr;
    srand(time(0) + usuario); // Semilla basada en el tiempo y el ID del usuario

    while (true) {
        int f = rand() % filas;
        int c = rand() % columnas;

        if (!asientos[f][c].ocupado) {
            asientos[f][c].ocupado = true;
            asientos[f][c].rect.setFillColor(sf::Color::Red); // Cambiar a rojo cuando se reserva
            std::cout << "Usuario " << usuario << " reservó la silla [" << f << ", " << c << "]." << std::endl;
            break; // Salir si el asiento fue reservado
        } else {
            std::cout << "Usuario " << usuario << " intentó reservar la silla [" << f << ", " << c << "] pero ya está ocupada." << std::endl;
        }
    }

    return nullptr;
}

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Reserva de Asientos");

    // Inicializar los asientos
    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            asientos[f][c].rect.setSize(sf::Vector2f(ancho, alto));
            asientos[f][c].rect.setFillColor(sf::Color::Green); // Asiento disponible
            asientos[f][c].ocupado = false;

            // Posición con pasillo en el medio
            int x = (c < 2) ? c * (ancho + espacio) + 100 : c * (ancho + espacio) + 100 + pasillo;
            asientos[f][c].rect.setPosition(x, f * (alto + espacio) + 50);
        }
    }

    // Crear hilos de usuarios
    const int numUsuarios = 5;
    pthread_t usuarios[numUsuarios];
    int ids[numUsuarios];

    for (int i = 0; i < numUsuarios; ++i) {
        ids[i] = i + 1;
        pthread_create(&usuarios[i], nullptr, reservar, &ids[i]);
    }

    // Esperar a que los hilos terminen
    for (int i = 0; i < numUsuarios; ++i) {
        pthread_join(usuarios[i], nullptr);
    }

    while (ventana.isOpen()) {
        sf::Event evento;
        while (ventana.pollEvent(evento)) {
            if (evento.type == sf::Event::Closed)
                ventana.close();
        }

        ventana.clear();

        // Dibujar los asientos
        for (int f = 0; f < filas; ++f) {
            for (int c = 0; c < columnas; ++c) {
                ventana.draw(asientos[f][c].rect);
            }
        }

        ventana.display();
    }

    return 0;
}
