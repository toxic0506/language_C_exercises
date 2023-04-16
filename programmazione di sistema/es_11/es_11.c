#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    int pid, p1p2[2];

    char stringa[15];
    char articolo_iniziale[50], articolo_finale[50], argomento_grep[100]];

    if (argc != 2)
    {
        printf("Numero argomenti sbagliato\n");
        exit(1);
    }

    pipe(p1p2);
    pid = fork();
    if (pid == 0)
    {
        while (1)
        {

            close(p1p2[0]);

            printf("Inserisci l'articolo che vuoi ricercare");
            scanf("%s", stringa);
            if (strcmp(stringa, "esci") == 0)
            {
                close(p1p2[1]);
                exit(0);
            }

            write(p1p2[1], stringa, sizeof(stringa));
        }
    }
    close(p1p2[1]);
    pid = fork();
    if (pid == 0)
    {

        read(p1p2[0], stringa, sizeof(stringa));
        sprintf(articolo_iniziale, "articolo. %d.", atoi(stringa));
        sprintf(articolo_finale, "articolo %d.", atoi((stringa) + 1));
        sprintf(argomento_grep, "-P '(?<=%s)(?s).*(?=%s)", articolo_iniziale, articolo_finale);

        execl("/usr/bin/grep", "-z", "-o", argomento_grep, argv[1], NULL);

        return -1;
    }
    close(p1p2[0]);
    wait(&pid);
    return 0;
}