#include "lib.h"


int main() {


    int playerHP = 20;
    int decision;
    int hasSword = 0;
    int monsterHP;

    printf("=== ?? Despertar en el Templo Prohibido ===/n");
    sleep();
    printf("Despiertas en un templo oscuro, con ecos antiguos.../n");
    sleep();
    printf("Te duele la cabeza y solo recuerdas tu nombre./n");
    sleep();

    printf("/nVes una espada oxidada en el suelo./n");
    sleep();
    printf("1. Tomar la espada/n");
    printf("2. Ignorarla y avanzar desarmado/n");
    sleep();
    printf("Elige: ");
    decision=scanf();

    if (decision == 1) {
        printf("Tomas la espada oxidada. Te sientes un poco mas seguro./n");
        hasSword = 1;
    } else {
        printf("Avanzas sin arma, decidido a enfrentar tu destino./n");
    }

    sleep();
    printf("/nCaminas por un pasillo largo y oyes un rugido.../n");
    sleep();
    printf("Un guardian de piedra aparece!/n");
    sleep();

    monsterHP = 15;

    while (monsterHP > 0 && playerHP > 0) {
        printf("/nTu HP: %d | Guardian HP: %d/n", playerHP, monsterHP);
        printf("1. Atacar/n");
        printf("2. Intentar huir/n");
        printf("3. Rogar piedad/n");
        printf("Elige: ");
        decision=scanf();
        sleep();

        if (decision == 1) {
            int damage;
            if (hasSword) {
                damage = 4 + rand() % 4; // 4 a 7
            } else {
                damage = 1 + rand() % 3; // 1 a 3
            }
            printf("Atacas y haces %d de danio!/n", damage);
            monsterHP -= damage;
        } else if (decision == 2) {
            printf("Intentas huir... pero el pasillo esta cerrado!/n");
            sleep();
            printf("Pierdes tiempo y el guardian ataca./n");
        } else if (decision == 3) {
            printf("Rogas piedad... pero el guardian no tiene alma./n");
            sleep();
            printf("Te ataca sin compasion./n");
        } else {
            printf("Te paralizas por el miedo.../n");
        }
        sleep();
        if (monsterHP > 0) {
            int monsterDamage = 3 + rand() % 3; // 3 a 5
            printf("El guardian te golpea y hace %d de dano!/n", monsterDamage);
            playerHP -= monsterDamage;
        }
    }

    if (playerHP <= 0) {
        printf("/nEl guardian de piedra te destruye./n");
        sleep();
        printf("Tu aventura termina en el templo prohibido.../n");
    } else {
        printf("/nHas derrotado al guardian!/n");
        sleep();
        printf("Encuentras una puerta secreta que lleva al exterior.../n");
        sleep();
        printf("Has escapado del templo prohibido!/n");
    }
    sleep();
    printf("/n=== Fin de la historia ===/n");
    return 0;
}
