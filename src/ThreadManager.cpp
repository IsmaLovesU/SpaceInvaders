#include "ThreadManager.h"
#include "GameEngine.h"
#include <chrono>
#include <thread>
#include <cstdlib>
#include <ctime>
#include <algorithm>

ThreadManager::ThreadManager(GameEngine* engine) 
    : gameEngine(engine), threadsRunning(false) {
    
    // Inicializar mutexes
    pthread_mutex_init(&entityMutex, nullptr);
    pthread_mutex_init(&scoreMutex, nullptr);
    pthread_mutex_init(&gameStateMutex, nullptr);
    pthread_mutex_init(&renderMutex, nullptr);
    
    // Inicializar semáforos
    sem_init(&playerActionSem, 0, 1);
    sem_init(&invaderActionSem, 0, 1);
    
    // Inicializar barrera (10 hilos participantes)
    pthread_barrier_init(&updateBarrier, nullptr, 10);
    
    // Inicializar variable de condición
    pthread_cond_init(&renderCondition, nullptr);
    
    // Inicializar generador de números aleatorios
    srand(time(nullptr));
}

ThreadManager::~ThreadManager() {
    if (threadsRunning) {
        stopThreads();
    }
    
    // Destruir mutexes
    pthread_mutex_destroy(&entityMutex);
    pthread_mutex_destroy(&scoreMutex);
    pthread_mutex_destroy(&gameStateMutex);
    pthread_mutex_destroy(&renderMutex);
    
    // Destruir semáforos
    sem_destroy(&playerActionSem);
    sem_destroy(&invaderActionSem);
    
    // Destruir barrera
    pthread_barrier_destroy(&updateBarrier);
    
    // Destruir variable de condición
    pthread_cond_destroy(&renderCondition);
}

void ThreadManager::startThreads() {
    threadsRunning = true;
    
    // Preparar datos para cada hilo
    for (int i = 0; i < 10; i++) {
        threadDataArray[i].engine = gameEngine;
        threadDataArray[i].threadId = i;
        threadDataArray[i].running = &threadsRunning;
    }
    
    // Crear los 10 hilos
    pthread_create(&playerMovementThread, nullptr, playerMovementFunc, &threadDataArray[0]);
    pthread_create(&playerShootingThread, nullptr, playerShootingFunc, &threadDataArray[1]);
    pthread_create(&invaderMovementThread, nullptr, invaderMovementFunc, &threadDataArray[2]);
    pthread_create(&invaderShootingThread, nullptr, invaderShootingFunc, &threadDataArray[3]);
    pthread_create(&bulletUpdateThread, nullptr, bulletUpdateFunc, &threadDataArray[4]);
    pthread_create(&collisionDetectionThread, nullptr, collisionDetectionFunc, &threadDataArray[5]);
    pthread_create(&renderThread, nullptr, renderFunc, &threadDataArray[6]);
    pthread_create(&inputHandlerThread, nullptr, inputHandlerFunc, &threadDataArray[7]);
    pthread_create(&scoreUpdateThread, nullptr, scoreUpdateFunc, &threadDataArray[8]);
    pthread_create(&gameStateThread, nullptr, gameStateFunc, &threadDataArray[9]);
}

void ThreadManager::stopThreads() {
    threadsRunning = false;
    
    // Esperar a que todos los hilos terminen
    pthread_join(playerMovementThread, nullptr);
    pthread_join(playerShootingThread, nullptr);
    pthread_join(invaderMovementThread, nullptr);
    pthread_join(invaderShootingThread, nullptr);
    pthread_join(bulletUpdateThread, nullptr);
    pthread_join(collisionDetectionThread, nullptr);
    pthread_join(renderThread, nullptr);
    pthread_join(inputHandlerThread, nullptr);
    pthread_join(scoreUpdateThread, nullptr);
    pthread_join(gameStateThread, nullptr);
}

// HILO 1: Movimiento del jugador
void* ThreadManager::playerMovementFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        // Esperar por el semáforo de acción del jugador
        sem_wait(data->engine->getThreadManager()->getPlayerActionSem());
        
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        
        if (data->engine->getGameState() == 0) { // Solo si está jugando
            // El movimiento real se maneja por input
            // Este hilo valida límites y actualiza posición
            Player* player = data->engine->getPlayer();
            if (player->entity.x < 1) player->entity.x = 1;
            if (player->entity.x >= data->engine->getScreenWidth() - 2) {
                player->entity.x = data->engine->getScreenWidth() - 3;
            }
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        
        sem_post(data->engine->getThreadManager()->getPlayerActionSem());
        
        // Esperar en la barrera
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 2: Disparos del jugador
void* ThreadManager::playerShootingFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        
        if (data->engine->getGameState() == 0 && data->engine->shouldPlayerShoot()) {
            Player* player = data->engine->getPlayer();
            std::vector<Entity>* bullets = data->engine->getPlayerBullets();
            
            if (bullets->size() < 3) {
                Entity bullet(player->entity.x, player->entity.y - 1, '^', 3);
                bullets->push_back(bullet);
                data->engine->setPlayerShoot(false);
            }
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 3: Movimiento de invasores
void* ThreadManager::invaderMovementFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    static int moveCounter = 0;
    static int direction = 1;
    
    while (*(data->running)) {
        sem_wait(data->engine->getThreadManager()->getInvaderActionSem());
        
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        
        if (data->engine->getGameState() == 0) {
            moveCounter++;
            
            if (moveCounter >= 30) { // Mover cada 30 frames
                moveCounter = 0;
                std::vector<Entity>* invaders = data->engine->getInvaders();
                
                bool shouldMoveDown = false;
                
                for (auto& invader : *invaders) {
                    if (invader.active) {
                        invader.x += direction;
                        
                        if (invader.x <= 1 || invader.x >= data->engine->getScreenWidth() - 2) {
                            shouldMoveDown = true;
                        }
                    }
                }
                
                if (shouldMoveDown) {
                    direction *= -1;
                    for (auto& invader : *invaders) {
                        if (invader.active) {
                            invader.y++;
                        }
                    }
                }
            }
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        
        sem_post(data->engine->getThreadManager()->getInvaderActionSem());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 4: Disparos de invasores
void* ThreadManager::invaderShootingFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    static int shootTimer = 0;
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        
        if (data->engine->getGameState() == 0) {
            shootTimer++;
            
            if (shootTimer >= 60) { // Disparar cada ~2 segundos
                shootTimer = 0;
                
                std::vector<Entity>* invaders = data->engine->getInvaders();
                std::vector<Entity>* bullets = data->engine->getInvaderBullets();
                
                if (!invaders->empty()) {
                    // Seleccionar un invasor aleatorio activo
                    std::vector<int> activeIndices;
                    for (size_t i = 0; i < invaders->size(); i++) {
                        if ((*invaders)[i].active) {
                            activeIndices.push_back(i);
                        }
                    }
                    
                    if (!activeIndices.empty()) {
                        int randomIdx = activeIndices[rand() % activeIndices.size()];
                        Entity bullet((*invaders)[randomIdx].x, 
                                    (*invaders)[randomIdx].y + 1, 'v', 2);
                        bullets->push_back(bullet);
                    }
                }
            }
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 5: Actualización de proyectiles
void* ThreadManager::bulletUpdateFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        
        if (data->engine->getGameState() == 0) {
            // Actualizar proyectiles del jugador
            std::vector<Entity>* playerBullets = data->engine->getPlayerBullets();
            for (auto it = playerBullets->begin(); it != playerBullets->end();) {
                it->y--;
                if (it->y < 1) {
                    it = playerBullets->erase(it);
                } else {
                    ++it;
                }
            }
            
            // Actualizar proyectiles de invasores
            std::vector<Entity>* invaderBullets = data->engine->getInvaderBullets();
            for (auto it = invaderBullets->begin(); it != invaderBullets->end();) {
                it->y++;
                if (it->y >= data->engine->getScreenHeight() - 1) {
                    it = invaderBullets->erase(it);
                } else {
                    ++it;
                }
            }
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 6: Detección de colisiones
void* ThreadManager::collisionDetectionFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        pthread_mutex_lock(data->engine->getThreadManager()->getScoreMutex());
        
        if (data->engine->getGameState() == 0) {
            std::vector<Entity>* playerBullets = data->engine->getPlayerBullets();
            std::vector<Entity>* invaderBullets = data->engine->getInvaderBullets();
            std::vector<Entity>* invaders = data->engine->getInvaders();
            Player* player = data->engine->getPlayer();
            
            // Colisiones proyectiles jugador vs invasores
            for (auto& bullet : *playerBullets) {
                for (auto& invader : *invaders) {
                    if (bullet.active && invader.active &&
                        bullet.x == invader.x && bullet.y == invader.y) {
                        bullet.active = false;
                        invader.active = false;
                        player->score += 10;
                    }
                }
            }
            
            // Colisiones proyectiles invasores vs jugador
            for (auto& bullet : *invaderBullets) {
                if (bullet.active && player->entity.active &&
                    bullet.x == player->entity.x && bullet.y == player->entity.y) {
                    bullet.active = false;
                    player->lives--;
                }
            }
            
            // Limpiar proyectiles inactivos
            playerBullets->erase(
                std::remove_if(playerBullets->begin(), playerBullets->end(),
                              [](const Entity& e) { return !e.active; }),
                playerBullets->end());
                
            invaderBullets->erase(
                std::remove_if(invaderBullets->begin(), invaderBullets->end(),
                              [](const Entity& e) { return !e.active; }),
                invaderBullets->end());
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getScoreMutex());
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 7: Renderizado
void* ThreadManager::renderFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getRenderMutex());
        
        data->engine->render();
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getRenderMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(33)); // ~30 FPS
    }
    
    return nullptr;
}

// HILO 8: Manejo de entrada
void* ThreadManager::inputHandlerFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        int ch = getch();
        
        if (ch != ERR) { // Solo procesar si hay una tecla presionada
            pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
            
            int currentState = data->engine->getGameState();
            
            // Manejar input según el estado del juego
            if (currentState == 0) { // Jugando
                switch (ch) {
                    case 'a':
                    case 'A':
                    case KEY_LEFT:
                        if (data->engine->getPlayer()->entity.x > 1) {
                            data->engine->getPlayer()->entity.x--;
                        }
                        break;
                        
                    case 'd':
                    case 'D':
                    case KEY_RIGHT:
                        if (data->engine->getPlayer()->entity.x < data->engine->getScreenWidth() - 2) {
                            data->engine->getPlayer()->entity.x++;
                        }
                        break;
                        
                    case 'w':
                    case 'W':
                    case ' ':
                        data->engine->setPlayerShoot(true);
                        break;
                        
                    case 'p':
                    case 'P':
                        pthread_mutex_lock(data->engine->getThreadManager()->getGameStateMutex());
                        data->engine->setGameState(1);
                        pthread_mutex_unlock(data->engine->getThreadManager()->getGameStateMutex());
                        break;
                        
                    case 'q':
                    case 'Q':
                    case 27: // ESC
                        data->engine->setRunning(false);
                        break;
                }
            } 
            else if (currentState == 1) { // Pausa
                switch (ch) {
                    case 'p':
                    case 'P':
                        pthread_mutex_lock(data->engine->getThreadManager()->getGameStateMutex());
                        data->engine->setGameState(0);
                        pthread_mutex_unlock(data->engine->getThreadManager()->getGameStateMutex());
                        break;
                        
                    case 'q':
                    case 'Q':
                    case 27: // ESC
                        data->engine->setRunning(false);
                        break;
                }
            }
            else if (currentState == 2 || currentState == 3) { // Game Over o Victoria
                switch (ch) {
                    case 'r':
                    case 'R':
                        data->engine->resetGame();
                        break;
                        
                    case 'q':
                    case 'Q':
                    case 27: // ESC
                        data->engine->setRunning(false);
                        break;
                }
            }
            
            pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        }
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 9: Actualización de puntaje y estadísticas
void* ThreadManager::scoreUpdateFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getScoreMutex());
        
        if (data->engine->getGameState() == 0) {
            // Aquí podrían agregarse bonificaciones por tiempo, combos, etc.
            // Por ahora solo verificamos el puntaje
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getScoreMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}

// HILO 10: Gestión del estado del juego
void* ThreadManager::gameStateFunc(void* arg) {
    ThreadData* data = static_cast<ThreadData*>(arg);
    
    while (*(data->running)) {
        pthread_mutex_lock(data->engine->getThreadManager()->getGameStateMutex());
        pthread_mutex_lock(data->engine->getThreadManager()->getEntityMutex());
        
        if (data->engine->getGameState() == 0) {
            Player* player = data->engine->getPlayer();
            
            // Verificar Game Over
            if (player->lives <= 0) {
                data->engine->setGameState(2);
            }
            
            // Verificar Victoria
            std::vector<Entity>* invaders = data->engine->getInvaders();
            bool allDestroyed = true;
            for (const auto& invader : *invaders) {
                if (invader.active) {
                    allDestroyed = false;
                    break;
                }
            }
            
            if (allDestroyed) {
                data->engine->setGameState(3);
            }
            
            // Verificar si los invasores llegaron al fondo
            for (const auto& invader : *invaders) {
                if (invader.active && invader.y >= data->engine->getScreenHeight() - 6) {
                    data->engine->setGameState(2); // Game Over
                    break;
                }
            }
        }
        
        pthread_mutex_unlock(data->engine->getThreadManager()->getEntityMutex());
        pthread_mutex_unlock(data->engine->getThreadManager()->getGameStateMutex());
        
        pthread_barrier_wait(&data->engine->getThreadManager()->updateBarrier);
        
        std::this_thread::sleep_for(std::chrono::milliseconds(16));
    }
    
    return nullptr;
}