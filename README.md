# Fplot

Un langage interpreté fortement influencé par [C](https://en.wikipedia.org/wiki/C_(programming_language) "C"),  [R](https://en.wikipedia.org/wiki/R_(programming_language)"R") et [Python](https://en.wikipedia.org/wiki/Python_(programming_language)"Python").

## Guide
_Voici un résumé des étapes mises en place._

### 1 Analyse lexicale
Avec **flex** on traduit le flux d'entrée _(lexèmes)_ en unités lexicales _(tokens)_.

### 2 Analyse syntaxique
Avec **Bison** on réalise l'analyse syntaxique du flux de token suivant notre propre grammaire.

### 3  Construction d'un arbre de syntaxe abstrait (AST)
L'arbre de syntaxe abstrait s'obtient facilement à partir des dérivations de notre analyse grammaticale du fait du choix d'une architecture à pile.  `$ make display_ast` permet la visualisation de l'AST.

### 4  Traduction en langage intermédiaire (pcodes)
Cette étape convertit l'AST en un langage d'assemblage executable dans notre partie opérationelle (PO). Celle ci est à jeu d'instructions réduit prenant 0 ou 1 argument _(ex: push_cst 42, pop, add)_.

### 5 Execution des pcodes dans une architecture à pile
Pour completer notre architecture de jeu d'instructions (ISA), nous simulons une architecture à pile permettant l'éxecution de nos pcodes. Nous retrouvons ici de la mémoire (programme, variables ...), une unité arithmétique et logique (ALU) basé sur des objet de tout types, un compteur ordinal...

## Usage
```
	make
	bin/main < fpsrc/euclide.fp
```
