import threading
import time

m_stoj = threading.Lock()
stoj = True

m_tazko_chory = threading.Lock()
m_lahko_chory = threading.Lock()
m_vysetrenie = threading.Lock()
cond_t = threading.Condition(m_vysetrenie)
cond_l = threading.Condition(m_vysetrenie)
count_tazko_chory = 0
count_lahko_chory = 0
count_vsetci = 0
pritomni_t = 0
pritomni_l = 0

def volno_lahko_chory(i):
    print("lahko chory pacient " + str(i) + " je doma");
    time.sleep(3);

def vysetrenie_lahko_choreho(i):

    global pritomni_l, pritomni_t, count_tazko_chory, count_lahko_chory, count_vsetci
    with m_vysetrenie:
        print(str(i) + " lahko chory sa dostavil pred miestnost ")
        while pritomni_t != 0 or pritomni_l >= 2:
            cond_l.wait()
        pritomni_l += 1
        print(str(i) + " lahko chory bude vstupovat do miestnostnosti")
    #cond_l.release()

    m_lahko_chory.acquire()
    print("vysetrenie " + str(i) + " lahko choreho")
    time.sleep(0.5)  # samotne vysetrenie v miestnosti u doktora
    count_lahko_chory += 1
    count_vsetci += 1
    m_lahko_chory.release()

    m_vysetrenie.acquire()
    pritomni_l -= 1
    print("lahko chory " + str(i) + " opustuje miestnostnost")
    cond_t.notifyAll()
    cond_l.notifyAll()
    m_vysetrenie.release()

def lahka_choroba(id):
    m_stoj.acquire()
    while stoj:
        m_stoj.release()

        vysetrenie_lahko_choreho(id)
        volno_lahko_chory(id)

        m_stoj.acquire()
    m_stoj.release()

def volno_tazko_chory(id):
    print("tazko chory pacient " + str(id) + " je doma")
    time.sleep(2.5)

def vysetrenie_tazko_choreho(i):
    # tazko chory vstupuje do miestnosti iba jeden a ma

    global pritomni_l, pritomni_t, count_tazko_chory, count_lahko_chory, count_vsetci

    #cond_t.acquire()
    with m_vysetrenie:
        pritomni_t+=1
        print(str(i) + " tazko chory sa dostavil pred miestnost")
        while pritomni_l != 0:
            cond_t.wait()
        print(str(i) + " tazko chory bude vstupovat do miestnostnosti")
    #cond_t.release()

    m_tazko_chory.acquire()
    print("vysetrenie " + str(i) + " tazko choreho")
    time.sleep(1)  #samotne vysetrenie v miestnosti u lekara
    count_tazko_chory+=1
    count_vsetci+=1
    m_tazko_chory.release()

    m_vysetrenie.acquire()
    pritomni_t-=1
    print("tazko chory " + str(i) + " opustuje miestnostnost")
    cond_l.notifyAll()
    m_vysetrenie.release()

def vazna_choroba(id):

    m_stoj.acquire()
    while stoj:
        m_stoj.release()

        vysetrenie_tazko_choreho(id)
        volno_tazko_chory(id)

        m_stoj.acquire()
    m_stoj.release()

def main():

    threads_t = []
    threads_l = []
    for i in range(0, 3, 1):
        threads_t.append(i)
        threads_t[i] = threading.Thread(target=vazna_choroba, args=(i,))
        threads_t[i].start()

    for i in range(0, 7, 1):
        threads_l.append(i)
        threads_l[i] = threading.Thread(target=lahka_choroba, args=(i,))
        threads_l[i].start()

    time.sleep(20)
    print "-------hlavne vlakno dospalo"

    global stoj, pritomni_l, pritomni_t

    m_stoj.acquire()
    stoj = False
    cond_l.notifyAll
    cond_t.notifyAll
    m_stoj.release()

    print "------idem joinovat"

    for i in range(0, 3, 1):
        threads_t[i].join()

    for i in range(0, 7, 1):
        threads_l[i].join()

    print("pocet vysetreni tazko chorych: " + str(count_tazko_chory) +" ,lahko_chorych: " + str(count_lahko_chory) + " vsetkych: " + str(count_vsetci))


if __name__ == '__main__':
    main()