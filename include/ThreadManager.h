#ifndef THREADMANAGER_H
#define THREADMANAGER_H

#include <pthread.h>
#include <semaphore.h>
#include <vector>
#include "GameEngine.h"

// Estructura para pasar datos a los hilos
struct ThreadData {
    GameEngine* engine;
    int threadId;
    bool* running;
};

class ThreadManager {
private:
    // Hilos del juego
    pthread_t playerMovementThread;
    pthread_t playerShootingThread;
    pthread_t invaderMovementThread;
    pthread_t invaderShootingThread;
    pthread_t bulletUpdateThread;
    pthread_t collisionDetectionThread;
    pthread_t renderThread;
    pthread_t inputHandlerThread;
    pthread_t scoreUpdateThread;
    pthread_t gameStateThread;
    
    // Mecanismos de sincronización
    pthread_mutex_t entityMutex;        // Protege acceso a entidades
    pthread_mutex_t scoreMutex;         // Protege el puntaje
    pthread_mutex_t gameStateMutex;     // Protege el estado del juego
    pthread_mutex_t renderMutex;        // Protege el renderizado
    
    sem_t playerActionSem;              // Semáforo para acciones del jugador
    sem_t invaderActionSem;             // Semáforo para acciones de invasores
    
    pthread_barrier_t updateBarrier;    // Barrera de sincronización para updates
    pthread_cond_t renderCondition;     // Variable de condición para renderizado
    
    // Datos compartidos
    ThreadData threadDataArray[10];
    GameEngine* gameEngine;
    bool threadsRunning;
    
    // Funciones estáticas para los hilos (requisito de pthreads)
    static void* playerMovementFunc(void* arg);
    static void* playerShootingFunc(void* arg);
    static void* invaderMovementFunc(void* arg);
    static void* invaderShootingFunc(void* arg);
    static void* bulletUpdateFunc(void* arg);
    static void* collisionDetectionFunc(void* arg);
    static void* renderFunc(void* arg);
    static void* inputHandlerFunc(void* arg);
    static void* scoreUpdateFunc(void* arg);
    static void* gameStateFunc(void* arg);
    
public:
    ThreadManager(GameEngine* engine);
    ~ThreadManager();
    
    void startThreads();
    void stopThreads();
    void pauseThreads();
    void resumeThreads();
    
    // Getters para los mutexes (usados por GameEngine)
    pthread_mutex_t* getEntityMutex() { return &entityMutex; }
    pthread_mutex_t* getScoreMutex() { return &scoreMutex; }
    pthread_mutex_t* getGameStateMutex() { return &gameStateMutex; }
    pthread_mutex_t* getRenderMutex() { return &renderMutex; }
    
    // Getters para semáforos
    sem_t* getPlayerActionSem() { return &playerActionSem; }
    sem_t* getInvaderActionSem() { return &invaderActionSem; }
    
    bool isRunning() const { return threadsRunning; }
};

#endif