
#ifndef _LISTE_DYNAMIQUE_H
#define _LISTE_DYNAMIQUE_H


#include "student.h"
#include <stdbool.h>
#include <stdlib.h>


typedef struct {
    student_t* valeurs;
    size_t taille_logique;
    size_t taille_physique;
} liste_t;

bool liste_init(liste_t* liste, size_t taille_initiale);
void liste_ajouter(liste_t* liste, student_t* element);
void liste_inserer(liste_t* liste, int indice, student_t* element);
bool liste_supprimer(liste_t* liste, int indice);
void liste_reset(liste_t* liste, size_t taille_initiale);
void liste_afficher(liste_t* liste);


#include <stdio.h>

bool liste_init(liste_t* liste, size_t taille_initiale) {
    if (taille_initiale == 0) {
        return false;
    }
    liste->taille_physique = taille_initiale;
    liste->taille_logique = 0;
    liste->valeurs = (student_t*)malloc(sizeof(student_t) * liste->taille_physique);
    if (!liste->valeurs) {
        perror("Erreur lors de l'allocation de la memoire!\n");
        exit(1);
    }
    return true;
}

/**
 * Cette fonction etend la taille physique de la liste si la taille logique
 * est superieure ou egale a la taille physique.
 */
void liste_etendre_taille(liste_t* liste) {
    if (liste->taille_logique >= liste->taille_physique) {
        liste->taille_physique *= 2;
        liste->valeurs = realloc(liste->valeurs, sizeof(student_t) * liste->taille_physique);
        if (!liste->valeurs) {
            perror("Erreur de malloc!\n");
            exit(1);
        }
    }
}

void liste_ajouter(liste_t* liste, student_t* element) {
    liste_etendre_taille(liste);
    liste->valeurs[liste->taille_logique] = *element;
    liste->taille_logique++;
}

void liste_inserer(liste_t* liste, int indice, student_t* element) {
    liste_etendre_taille(liste);
    // On va parcourir la liste a l'envers, de la fin jusqu'a l'indice demande
    // en deplacant les valeurs vers la droite.
    for (int i = liste->taille_logique; i > indice; i--) {
        liste->valeurs[i] = liste->valeurs[i - 1];
    }
    // Finalement, on ajoute la valeur a l'indice demande.
    liste->valeurs[indice] = *element;
    liste->taille_logique++;
}

bool liste_supprimer(liste_t* liste, int indice) {
    if (indice >= liste->taille_logique) {
        return false;
    }
    liste->taille_logique--;
    // On va parcourir la liste en ecrasant la valeur i par la valeur i+1 pour ne pas avoir de "trou" dans le tableau.
    for (int i = indice; i < liste->taille_logique; i++) {
        liste->valeurs[i] = liste->valeurs[i + 1];
    }
    return true;
}

void liste_reset(liste_t* liste, size_t taille_initiale) {
    free(liste->valeurs);
    liste_init(liste, taille_initiale);
}

void liste_afficher(liste_t* liste) {
    char* buff = (char*)malloc(1000*sizeof(char));
    printf("%zd elements dans la liste.\n", liste->taille_logique);
    for (int i = 23605; i <= 23612; i++) {

        student_to_str(buff, &(liste->valeurs[i]));
        printf("%s\n",buff);
    }
    free(buff);
    
}


#endif  // _LISTE_DYNAMIQUE_H
