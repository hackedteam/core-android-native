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
 - ROOT_SERVER: path in cui verrà copiato il binario avviato come servizio (di default l'avrei messo in /system/bin/event_handlerd)
 - ROOT_CLIENT: path in cui verrà copiato il binario da usare come client (solitamente /system/bin/rilcap)
 - INSTALL_SCRIPT: script di avvio al boot
 - INSTALL_SCRIPT_BAK: copia del binario da sostituire
 - INSTALL_REC_SCRIPT: script install-recovery.sh
 - INSTALL_REC_SCRIPT_BAK: backup di install-recovery.sh in caso lo su usi

Per avere i log a disposizione tramite logcat editare il file headers/log.h commentando opportunamente "#define DEBUG" sulla nuova shell.
Per la vecchia shell il #define DEBUG si trova direttamente in suidext.c






***********################################*************
***********####### COMPILAZIONE #########*************
***********################################*************

Compilare con il comando "build.sh" e non direttamente tramite ndk-buid. Assicurarsi di avere come working directory la root del codice.
Vengono creati i seguenti file:

 - selinux_exploit: l'exploit basato su put_user
 - selinux4_exploit: l'exploit towelroot
 - selinux_suidext: la nuova shell che comprende sia il daemon, sia il client
 - selinux_check: esegue un check sull'exploitabilità del device. Restituisce 1 in caso di exploitabilità, 0 altrimenti.
 - runner: è il binario che lancia il demone di root. E' integrato direttamente in suidext.
 - suidext: shell suid vecchia per Android <= 4.1
 
 - expl_check: check di exploitabilità tramite exploit legacy
 - local_exploit: binario lancia tutti gli exploit disponibili
 - suidext: shell legacy



***********################################*************
***********####### ESECUZIONE     #########*************
***********################################*************


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

Per avviare la shell in modalità daemon eseguire "selinux_suidext --daemon" oppure eseguire "selinux_suidext rt" che avvierà direttamente il demone effettuando un'installazione persistente.
Il client rispetta le specifiche della vecchia shell.

*** SHELL LEGACY***

Semplice file setuid.
