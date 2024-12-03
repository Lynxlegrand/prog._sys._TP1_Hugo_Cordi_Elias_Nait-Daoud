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




