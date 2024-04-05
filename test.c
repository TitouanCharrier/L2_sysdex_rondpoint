#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

int main() {
    char chaine[] = "123abc456def789"; // Exemple de chaîne contenant des nombres et des lettres
    int nombre = 0;
    char lettre = '\0';
    int i;

    for (i = 0; chaine[i] != '\0'; i++) {
        if (isdigit(chaine[i])) {
            // Si le caractère est un chiffre, mettez à jour le nombre
            nombre = nombre * 10 + (chaine[i] - '0');
        } else if (isalpha(chaine[i])) {
            // Si le caractère est une lettre, mettez à jour la lettre
            lettre = chaine[i];
        }
        // Vous pouvez ajouter d'autres conditions pour d'autres types de caractères si nécessaire
    }

    // Une fois que vous avez analysé toute la chaîne, vous pouvez utiliser les valeurs obtenues
    printf("Nombre extrait : %d\n", nombre);
    printf("Lettre extrait : %c\n", lettre);

    return 0;
}
