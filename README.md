# parallel-programing

Simulujte nasledujucu situaciu.
Vo vyvhodnej Europe, v malej dedinke prepukla epidemia a nakazilo sa 10 ludi. Sedem z nich maju len mensiu nemoc, no traja ludia maju vazne problemi. Aby sa nakazilo este viac ludi, tak sa vlada rozhodla tychto 10 ludi prestahovat do vedlasej dedinky, kde uz nikto dlhsie nebyval. Tito ludia musia kazdy den chodit na zdravotne stredisko v pravidelnych intervaloch (lahko chori v simulacii 3s a takzko chori 2.5s), kde ordinuje jeden doktor. Osetrenie jedneho pacienta trva urcity cas (lahko choreho v simulacii 0.5s a tazko choreho 1) a potom moze ist domov. Cela simulacia trva 20s.

1. Doplnte do programu pocitadlo pocitajuce, kolko krat boli vysetreni pacienti s velkymi ocherenim a tiez pocitadlo ratajucem kolko krat boli vysetreni pacienti len sa malou nemocou.

2. Zabezpecte, aby do miestnosti k lekarovi mohli vojst maximalne dvaja lahko postihnuti pacienti, a ak v cakarni caka pacient z vaznym postihnutim, tak lekar dovysetruje pritimnich pacientov v miestnosti a pacient z vaznym postinutim ma prednst pred ostatnymi. Pocet pacientov v miestnosti s vaznym ochorenim mozu byt aj 3 ale vysetruje sa iba jeden.

3. Osetrite v programe spravne ukoncenie simulacie po uplynuti stanoveneho casu, tak ze ti pacienti co su este v cakarni, tych	 doktor dovysetruje. 

Poznamky:
- na synchronizaciu je mozne pouzit mutexy, podmienene premenne alebo semafory
- nespoliehajte sa na uvedene casy, simulacia by mala fungovat aj s inymi casmi
- build (console): gcc epidemia.c -o epidemia -lpthread

