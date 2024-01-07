# Projet VAL - Antonin DUMAS- Aymeric DROULERS

## Déscription du projet

L'objectif de notre projet était de modéliser et simuler la gestion des rames d'un métro.
Nous avons choisi de représenter la ligne 1 du métro lillois.
L'objectif principal est de modéliser la gestion des rames de métro, en mettant en œuvre une supervision globale (le superviseur), un fonctionnement autonome pour chaque rame et une gestion indépendante des stations. 


## POO

Nous avons réalisée notre projet en utlisant la POO, et en spéarent chaque partie de notre code en classes.

Nous pouvons identifier quatre classes principales : la classe rame, qui gère une rame de métro, la classe station, qui gère chaque station, la classe troncon qui gère chaque troncon de ligne entre deux stations et enfin la classe superviseur, qui gère l'ensemble du réseau.

De plus nous avons chercher à appliquer le principe de résponsabilitée unique, c'est à dire que chaque fonction a une utlité bien précise et qu'elle ne fait que ca, ce qui rend le code plus lisible et maintenable.
"Fais une seule chose, mais fais-la bien."

### Rame

La classe rame est définie par un numéro, le tronçon sur lequel elle circule, la distance qu’elle a parcourue sur ce tronçon, sa vitesse, le nombre de passagers qu’elle transporte, et sa position. Nous avons développés ces méthodes getters et setters ainsi que d’autres fonctions nécessaires au bon fonctionnement du programme. Par exemple nous avons développé une méthode qui indique si la rame est en sécurité par rapport aux autres, une qui gère la vitesse ou une autre qui gère les changements de passagers.


### Station

La classe station contient un nom, le nombre de personnes dans la station, si la station est le terminus de la ligne et la position de la station. Nous avons développé ces méthodes getters et setters pour le bon fonctionnement de l’ensemble. 

### Troncon

La classe troncon relie deux stations. Elle contient donc une réference à chacune de ces stations, ainsi que la distance du troncon. Elle contient également un attribut listeRames qui contient comme son nom l'indique la liste des rammes sur le troncon.

### Lignes

Nous avons crée une class "bonus" lignes, qui permet de regrouper les troncons et les stations en fonction de la ligne. Elle n'a pas de méthodes autres que ses setters et getters.

### Superviseur

Nous avons enfin crée une classe supérivseur qui regroupe l'ensemble du réseau, avec les lignes, les rames et les stations. Il est passé en argument de la fonction affichage, qui récupère ainsi le réseau dans son état et peut l'afficher.

## Bibiliothèques

Nous avons utilisé la bibliothèque CMAKE pour que notre projet tourne sur les différents OS (on ne garentit pas que ca tourne sur votre grille pain), comme demandé. Nous avons également utilisé pour la partie affichage la bibliothèque SFML.

## Threads

Pour faire tourner les différentes parties de notre code en même temps nous avons utilisé des threads. Nous avons crée un premier thread qui gère la partie logique du métro, et un second qui gère la parte affichage de la simulation.