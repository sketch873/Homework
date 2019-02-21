Andrei-Costin Martin
336CA

    Implementeare Miner:
    - Am folosit un semafor cu o permisiune;
    - Obtine permisiunea, face doua citiri si elibereaza semaforul
    - Cazurile de baza sunt EXIT, NULL, END
    - Pe cazurile de baza elibereaza semaforul si se intoarce la prima citire
    - Altfel face a doua citire si calculeaza hash-ul corect

    Implementare CommunicationChannel:

    *putMessageMinerChannel*
    - e facut synchronized deoarece mereu trebuie trimis un mesaj dar
    pot fi mai multe procese
    - da dreptul vrajitorilor sa citeasca (semafor)

    *getMessageMinerChannel*
    - daca pot trece de semafor atunci citesc
    - incerc sa obtin accestul pentru semaforul 'letWrite'

    *putMessageWizzardChannel*
    - daca mesajul este de tipul 'NO_PARENT' atunci
    eliberez semaforul si il ocup(pentru a lasa si un alt thread)
    - daca mesajul este de tipul 'END' atunci eliberez semaforul
    (conditia fiind sa fie maxim o permisiune de trecere)
    - altfel primesc mesaje (thread-urile o sa fie blocate
    in cazul 'NO_PARENT' sau in *getMessageMinerChannel*)

    *getMessageWizzardChannel*
    - doar daca trec de semafor pot citi
    - nu se pun conditii suplimentare deoarece au fost puse
    in clasa Miner
