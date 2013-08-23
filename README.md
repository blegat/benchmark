Benchmark
=========

`benchmark.c` et `benchmark.h` contiennent les outils de benchmark
Chacun des exercice est dans `<prog>.c`.
Pour compiler, il y a un `Makefile` générique qui inclu
`<prog>.mk` qui définit des constantes propres à `<prog>`.
Il y a un `<prog>.gpi` qui fournit le code pour plotter les `.csv`
produit par `<prog>`.
Dans le `Makefile`,
il y a les règles pour compiler en `.o`,
créer `<prog>`,
créer les `.csv`, les afficher,
sauver la figure en `.png` et `clean`.
Pour compiler un certain exercice,
il suffit de décommenter l'`include` correspondant dans le Makefile.

J'ai essayé d'avoir une structure qui rend le Makefile
assez utile tout en restant simple.
Pour présenter cela en exercice,
il faudrait retirer une partie de `<prog>.c` pour laisser aux étudiants trouver,
j'ai noté dans chacun une idée avec le début de la partie à
retirer à `// BEGIN` et la fin à `// END`.
Après, j'hésite entre donner le Makefile en effectuant déjà l'include et
donner un Makefile différent à chaque fois et donc
avoir plein de fichiers séparés à télécharger
ou garder exactement la structure que j'ai et leur demander de télécharger
tout une fois en `tar` et de toujours travailler dans ce dossier comme je fais.
Je pencherai vers la deuxième solution car ça éviterait que les étudiants
perde du temps à retélécharger quelque chose,
devoir tout copier/décompresser à un endroit,
comprendre ce que chaque fichier fait, etc... à chaque fois.
En plus, on pourra leur proposer d'utiliser Git car ils reviendront sur
ce dossier semaine après semaine et ils verront comment Git rend ça clair.
On pourra leur dire la première semaine, téléchargez tout,
puis faire `git init` puis à la fin de l'énoncé on dit,
faite `git commit -a -m "Fin de l'exercice 3.0"`, etc...
On peut même imaginer que le code soit sur un repo de l'UCL comme
pour les projet ainsi il devrait faire un `git clone` et si on désire
changer quelque chose à `benchmark.c` ou quoi, on leur dira de faire
`git fetch`, `git merge` et ils verront à quel point c'est pratique.
