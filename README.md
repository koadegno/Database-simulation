
# Projet du cours de systeme d'exploitation 

Projet :
  Simulation d'une base de donneé d'étudiant. Le but du projet est de pouvoir lancer des commandes a partir d'un fichier
  ou simplement dans le terminale.

Commandes : 
  * select \<champ\>=\<valeur\>. Cette requête renvoie la liste des étudiants qui corres-
    pondent à l’unique filtre \<champ\>=\<valeur\>.
  * insert \<fname\> \<lname\> \<id\> \<section\> \<birthdate\>. Cette requête insère
un nouvel étudiant dans la base de données.
  * delete \<champ\>=\<valeur\>. Cette requête supprime tous les étudiants qui corres-
pondent au filtre donné.
  * update \<filtre\>=\<valeur\> set \<champ_modifie\>=\<valeur_modifiee\>. Cette
requête modifie tous les étudiants correspondant au filtre \<filtre\>=\<valeur\>, en
donnant la valeur \<valeur_modifiee\> au champ \<champ_modifie\>.

Lancer le projet: 
- ./tinydb "<queries.txt"
- ./tinydb 

Pour plus d'information sur les requêtes : projet_tinydb.pdf
