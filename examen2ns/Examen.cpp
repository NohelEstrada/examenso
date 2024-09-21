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

// Mutex
pthread_mutex_t asientoMutex;

struct Silla {
    sf::RectangleShape rect;
    bool ocupado; // Estado de la silla
};

// Vector global para los asientos
std::vector<std::vector<Silla>> asientos(filas, std::vector<Silla>(columnas));

// Función de reserva para hilos
void* reservar(void* usuarioPtr) {
    int usuario = (int )usuarioPtr;
    srand(time(0) + usuario); // Semilla para la reserva

    while (true) {
        int f = rand() % filas;
        int c = rand() % columnas;

        // Bloquear acceso a la sección crítica
        pthread_mutex_lock(&asientoMutex);

        if (!asientos[f][c].ocupado) {
            asientos[f][c].ocupado = true;
            asientos[f][c].rect.setFillColor(sf::Color::Red); // Asiento reservado
            std::cout << "Usuario " << usuario << " reservó la silla [" << f << ", " << c << "]." << std::endl;
            std::cout.flush();
            pthread_mutex_unlock(&asientoMutex);
            break; // Finaliza si se reservó
        } else {
            std::cout << "Usuario " << usuario << " intentó reservar la silla [" << f << ", " << c << "] pero ya está ocupada." << std::endl;
            std::cout.flush();
        }

        pthread_mutex_unlock(&asientoMutex);
    }

    return nullptr;
}

int main() {
    sf::RenderWindow ventana(sf::VideoMode(800, 600), "Reserva de Asientos");

    // Inicializar asientos
    for (int f = 0; f < filas; ++f) {
        for (int c = 0; c < columnas; ++c) {
            asientos[f][c].rect.setSize(sf::Vector2f(ancho, alto));
            asientos[f][c].rect.setFillColor(sf::Color::Green); // Silla disponible
            asientos[f][c].ocupado = false;

            // Posicionar asiento
            int x = (c < 2) ? c * (ancho + espacio) + 100 : c * (ancho + espacio) + 100 + pasillo;
            asientos[f][c].rect.setPosition(x, f * (alto + espacio) + 50);
        }
    }

    // Iniciar mutex
    pthread_mutex_init(&asientoMutex, nullptr);

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
            
            // Clic del mouse
            if (evento.type == sf::Event::MouseButtonPressed && evento.mouseButton.button == sf::Mouse::Left) {
                // Obtener la posición del clic
                sf::Vector2f posicionMouse = ventana.mapPixelToCoords(sf::Mouse::getPosition(ventana));

                // Verificar si el clic fue sobre un asiento
                for (int fila = 0; fila < filas; ++fila) {
                    for (int columna = 0; columna < columnas; ++columna) {
                        if (asientos[fila][columna].rect.getGlobalBounds().contains(posicionMouse)) {
                            pthread_mutex_lock(&asientoMutex);

                            if (!asientos[fila][columna].ocupado) {
                                asientos[fila][columna].ocupado = true; // Marcar reservado
                                asientos[fila][columna].rect.setFillColor(sf::Color::Red); // Cambiar color
                                std::cout << "Asiento [" << fila << ", " << columna << "] reservado por clic." << std::endl;
                            } else {
                                std::cout << "El asiento [" << fila << ", " << columna << "] ya está reservado." << std::endl;
                            }

                            pthread_mutex_unlock(&asientoMutex);
                        }
                    }
                }
            }
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