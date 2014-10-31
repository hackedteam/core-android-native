***********################################*************
***********####### SELINUX_LEGACY #########*************
***********################################*************

Contiene tutto il codice nativo per le versioni di Android senza SELinux (Android <= 4.1)

###### Contenuto ######

- suidext: contiene la shell standard (versione setuid)
- local2root: contiene tutti gli exploit local2root per le versioni di Android <= 4.1
- headers: include necessari per la compilazione





***********################################*************
***********####### SELINUX_NATIVE #########*************
***********################################*************

Contiene tutto il codice nativo per le ultime versioni di Android con SELinux attivo (Android >= 4.2.2)


###### Contenuto ######

- put_user_exploit: contiene l'exploit basato sulla put_user
- kernel_waiter_exploit: contiene l'exploit towelroot
- suidext: contiene la nuova shell demonizzata
- utils: librerie varie
- headers: include necessari per la compilazione





***********################################*************
***********####### CONFIGURAZIONE #########*************
***********################################*************

La configurazione della shell di root è descritta in shell_params.h . Questo file è comune ad entrambe le shell.

 - SHELL_PORT: porta su cui il servizio root andrà in ascolto e su cui la parte client si connetterà
 - ROOT_BIN: path d'installazione del binario che esegue il processo root. Attualmente /system/bin/ddf. 
   	     E' utilizzato sia come server al boot che come client per richiedere l'esecuzione di comandi.
 - INSTALL_SCRIPT: script di avvio al boot (wrapper a /system/bin/debuggerd)
 - INSTALL_SCRIPT_BAK: copia del binario da sostituire
 - INSTALL_REC_SCRIPT: script install-recovery.sh
 - INSTALL_REC_SCRIPT_BAK: backup di install-recovery.sh in caso lo su usi

Per avere i log a disposizione tramite logcat editare il file headers/log.h commentando opportunamente "#define DEBUG" sulla nuova shell.
Per la vecchia shell il #define DEBUG si trova direttamente in suidext.c




***********################################*************
***********####### COMPILAZIONE #########*************
***********################################*************

Compilare con il comando "build.sh" e non direttamente tramite ndk-buid. Assicurarsi di avere come working directory la root del codice.
Il sistema di build genera in automatico gli exploit locali in tre versioni diverse: local, remote e shared library.

1) local: file necessari per eseguire gli exploit locali tramite l'apk.
2) remote: exploit da lanciare dalla sandbox del browser.
3) shared library: è la libreria che contiene la funzione per eseguire gli exploit da invocare tramite dlopen.

I corrispettivi file vengono creati in tre cartelle diverse:

1) bin/local: 

 - selinux_exploit: l'exploit basato su put_user
 - selinux4_exploit: l'exploit writeup di towelroot
 - selinux_suidext: la nuova shell che comprende sia il daemon, sia il client
 - selinux_check: esegue un check sull'exploitabilità del device. Restituisce 1 in caso di exploitabilità, 0 altrimenti.
 - runner: è il binario che lancia il demone di root. E' integrato direttamente in selinux_suidext.
 - suidext: shell suid vecchia per Android <= 4.1
 - expl_check: check di exploitabilità tramite exploit legacy
 - local_exploit: binario che lancia tutti gli exploit legacy disponibili


2) bin/remote
 - selinux_exploit: contiene in un unico binario i due exploit selinux e la shell di root.

3) bin/shared_lib
 - libselinux_exploit.so: shared library contenente i due exploit selinux e la shell di root.
  



***********################################*************
***********####### ESECUZIONE     #########*************
***********################################*************

###########################
##### 1) LOCAL EXPLOIT: ###
###########################

*** EXPLOIT SELINUX***

Deve essere lanciato passando due argomenti:

     - argv[1]: path assoluto alla nuova shell
     - argv[2]: path assoluto alla vecchia shell

L'exploit installerà automaticamente la shell vecchia se possibile o la shell nuova in base alle rilevazioni effettuate sul device.
Se l'exploit ha successo /system/bin/<shell> sarà presente ed utilizzabile in modo trasparente.

*** EXPLOIT SELINUX 4***

Deve essere lanciato passando un solo argomento

     - argv[1]: path assoluto alla nuova shell

Se l'exploit ha successo /system/bin/<shell> sarà presente ed utilizzabile in modo trasparente.


*** EXPLOIT LEGACY***

Deve essere lanciato passando una stringa come argomento che rappresenza il comando da eseguire.
NB: Alcuni exploit richiedono l'accesso a device video legati alla gesione della camera. 
    E' necessario che tali device siano accessibili in scrittura in base alla sandbox di esecuzione (privilegio CAMERA).

*** SHELL SELINUX***

Per avviare la shell in modalità daemon eseguire "selinux_suidext --daemon" oppure eseguire "selinux_suidext rt" (da root) che avvierà direttamente il demone effettuando un'installazione persistente.
Il client rispetta le specifiche della vecchia shell.

*** SHELL LEGACY***

Semplice file setuid.


############################
##### 2) REMOTE EXPLOIT: ###
############################

Va semplicemente lanciato passando come unico parametro l'apk da installare al termine dell'exploit. E' totalmente automatico.
L'exploit è asincrono di modo da non tenere il browser impegnato. Ritorna 1 se il device risulta vulnerabile e di conseguenza l'exploit viene almeno provato, 0 altrimenti.


####################################
##### 3) SHARED LIBRARY EXPLOIT: ###
####################################

E' una libreria che va caricata tramite dlopen e risolto il simbolo che effettua l'exploit. Il simpolo attuale è "exploit".
La funzione va chiamata senza argomenti. Ritorna 1 exploit eseguito con successo, 0 altrimenti.



