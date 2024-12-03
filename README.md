# prog._sys._TP1_Hugo_Cordi_Elias_Nait-Daoud
TP1 de programmation système de Hugo Cordi et Elias Nait Daoud 2 G2 TD2 TP3
## 1. Affichage d’un message d’accueil, suivi d’un prompt simple.

![image](https://github.com/user-attachments/assets/42db0cc5-d4f0-414c-9a3f-caf6fb26d801)

Ce code implémente un shell minimaliste en C. Il affiche un message de bienvenue, puis entre dans une boucle infinie où il affiche une invite de commande (`enseash %`) et lit les entrées utilisateur via l'appel système `read`. Les entrées sont formatées en chaînes valides en remplaçant le caractère de nouvelle ligne par un caractère nul.  Aucune commande n'est exécutée, ce qui rend ce shell purement interactif mais non fonctionnel pour exécuter des commandes.

## 2. Exécution de la commande saisie et retour au prompt

![image](https://github.com/user-attachments/assets/0157b94a-f211-4c95-99ca-e12c7081bb8e)

Ce code améliore le shell précédent en ajoutant plusieurs fonctionnalités. Il introduit la commande `fortune`, qui affiche un message fixe et l'heure actuelle. Le shell peut également exécuter des commandes système saisies par l'utilisateur grâce à la création de processus fils via `fork`. Les commandes et leurs arguments sont analysés à l'aide de `strtok` avant d'être exécutés avec `execlp`. Le processus parent attend la fin du processus fils avec `waitpid`. Ces améliorations rendent le shell fonctionnel pour exécuter des commandes simples, tout en maintenant une structure interactive et minimaliste.

## 3. Gestion de la sortie du shell avec la commande “exit” ou un ctrl + D;

![image](https://github.com/user-attachments/assets/adbb7a02-7ad3-43c4-920f-fdff85f45d9f)

Ce code rajoute la commande `exit` qui permet de quitter la console. On peut obtenir le même résultat avec ctrl + D. 

## 4. Affichage du code de retour (ou du signal) de la commande précédente dans le prompt :

![image](https://github.com/user-attachments/assets/11ced065-c7e5-47ba-94ee-34eacbabeb86)

Après l'exécution d'une commande, le prompt devient dynamique et affiche des informations sur l'état de sortie du processus fils, telles que le code de retour ou le signal ayant causé sa terminaison. Ces modifications enrichissent l'expérience utilisateur et renforcent la fonctionnalité et la robustesse du shell.

## 5. Mesure du temps d’exécution de la commande en utilisant l’appel clock_gettime 

![image](https://github.com/user-attachments/assets/af78a4d9-b73e-46f1-9e0d-5557cfacaec9)

Les modifications apportées au programme incluent la mesure du temps d'exécution de chaque commande. Après que le processus fils ait terminé son exécution, le programme calcule le temps écoulé en millisecondes à l'aide de `clock_gettime()` avec l'horloge `CLOCK_MONOTONIC`, et l'affiche dans le prompt. Si la commande s'est terminée normalement, le code de sortie et le temps d'exécution sont affichés, par exemple : `[exit:0|123ms]`. Si la commande a été interrompue par un signal, le signal et le temps d'exécution sont affichés, comme `[sign:9|150ms]`. Cette fonctionnalité permet de visualiser non seulement l'état de la commande mais aussi son temps d'exécution.


