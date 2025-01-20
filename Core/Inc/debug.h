/*
 * Equipe: Thales Tenebra, Silvio Porto, Antonio
 * Arquivo: debug.h
 * Descrição: Habilita ou desabilita mensagens de debug no sistema.
 */

#ifndef DEBUG_H
#define DEBUG_H

#include <stdio.h>

// Ativar ou desativar debug
#define DEBUG 1

#if DEBUG
#define DEBUG_PRINT(...) printf(__VA_ARGS__)
#else
#define DEBUG_PRINT(...)
#endif

#endif // DEBUG_H
