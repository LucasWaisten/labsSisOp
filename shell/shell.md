# Lab: shell

### Búsqueda en $PATH

- <p style="color:#096F93";>Pregunta:</p>¿Cuáles son las diferencias entre la syscall execve(2) y la familia de wrappers proporcionados por la librería estándar de C (libc) exec(3)?

- <p style="color:#096F93";>Respuesta:</p> *execve(2)* ejecuta el programa referenciado en el *pathname*. Esto genera que se ejectue un nuevo programa (inicializado o no) en el lugar donde se efectuo la llamada de la syscall. Algo que se cree, y que es muy enganioso, es que con la llamada *execve(2)* se ejecuta un nuevo proceso, lo cual no es cierto ya que durante el proceso de la llamada se conservan sin cambios muchos atributos, en particular su PID. Como conclusion, *execve()* organiza un proceso existente para ejecturar un nuevo programa.<br> La familia de wrappers *exec(3)* reemplaza el proceso imagen ejecutado por uno nuevo, utilizando *execve(2)*. Al ser wrappers, los mismos se abstienen de detalles de implementacion para facilital la escritura de programas. Se proporcionan disntintas interfaces que logran externder la funcionalidad que se obtiene al usar la syscall *execve()*


- <p style="color:#096F93";>Pregunta:</p> ¿Puede la llamada a exec(3) fallar? ¿Cómo se comporta la implementación de la shell en ese caso?

- <p style="color:#096F93";>Respuesta:</p>La llamada a *exec(3)* si puede fallar, sacado textual del manual, todas la familia de wrappers fallara de acorde al fallo que se produzca en *execve(2)*. El fallo de *execve(2)* consta en devolver -1 e indicar a *errno* el error especificado. Como conclusion podemos decir que la shell esperara el error de *execve(2)* para informar el fallo de la familia *exec(3)*
---

### Comandos built-in

- <p style="color:#096F93";>Pregunta:</p> ¿Entre cd y pwd, alguno de los dos se podría implementar sin necesidad de ser built-in? ¿Por qué? ¿Si la respuesta es sí, cuál es el motivo, entonces, de hacerlo como built-in? (para esta última pregunta pensar en los built-in como true y false)

- <p style="color:#096F93";>Respuesta:</p><p> Entre los comandos mencionado se podria implementar sin ser built-in <code>pwd</code>. Basicamente <code>pwd</code> obtiene el path donde uno se ecuentra parado en bash, el mismo puede pertenecer a shell como un programa o se puede implementar, sin pertenecer a shell, como un comando binario, en ambos casos el resultado seria el mismo. Una ventaja de hacerlo built-in seria la de ahorrarnos un llamado fork() y un llamado exec(), ambos llamados son costosos, en cambio si se ejecuta como un programa de shell no seria necesario hacer esos llamados, ergo, menos costo a la hora de ejecutar.<br> En caso de <code>cd</code> es necesario que se ejecute como un built-in ya que se requiere modificar la direccion de la shell y no de un proceso hijo.
</p>

---
### Variables de entorno temporarias
- <p><p style="color:#096F93">Pregunta:</p>¿Por qué es necesario hacerlo luego de la llamada a fork(2)?<br></p>

- <p style="color:#096F93";>Respuesta:</p> Porque al ser variables temporales, solo deben existir en la ejecución del programa y no en la shell misma. Por lo tanto, se deben setear luego del fork y antes de execv, para que solo esten definidas en el proceso que se va a transformar en el programa a ejecutar.

<p style="color:#096F93">En algunos de los wrappers de la familia de funciones de exec(3) (las que finalizan con la letra e), se les puede pasar un tercer argumento (o una lista de argumentos dependiendo del caso), con nuevas variables de entorno para la ejecución de ese proceso. Supongamos, entonces, que en vez de utilizar setenv(3) por cada una de las variables, se guardan en un array y se lo coloca en el tercer argumento de una de las funciones de exec(3).</p>

- <p> <p style="color:#096F93";>Pregunta:</p> ¿El comportamiento resultante es el mismo que en el primer caso? Explicar qué sucede y por qué.</p>

- <p style="color:#096F93";>Respuesta:</p>El comportamiento no es el mismo, lo que sucede es que el proceso que ejecuta el programa solo tiene como variables de entorno a las recibidas como argumento como variables de entorno temporarias (las almacenadas en cmd->eargv).

- <p><p style="color:#096F93";>Pregunta:</p> Describir brevemente (sin implementar) una posible implementación para que el comportamiento sea el mismo.</p>

- <p style="color:#096F93";>Respuesta:</p>Una posible implementación puede ser usar la variable global `extern char **environ`, que es un array con las variables de entorno. Agregándole las variables de entorno temporales recibidas como argumento, al pasárselo como parámetro a una de las funciones de la familia de funciones de exec(3) (las que finalizan con la letra e) obtendría el comportamiento esperado.

---
### Procesos en segundo plano
- <p style="color:#096F93";>Pregunta:</p> Detallar cuál es el mecanismo utilizado para implementar procesos en segundo plano.

- <p style="color:#096F93";>Respuesta:</p><p>El mecanismo utilizado consiste en hacer un llamado al proceso almacenado en <code>backcmd->c</code> y para enviarlo de forma recursiva a la funcion <code>exec_cmd</code>. Al hacer esto el proceso se ejecuta de manera normal, en caso de que haya un proceso ejecutandose en segundo plano, la shell hara esperar al comando ejecutado de manera no bloqueante para luego proceder con el mismo</p>
---

### Flujo estándar
- <p style="color:#096F93";>Pregunta:</p> Investigar el significado de 2>&1, explicar cómo funciona su forma general y mostrar qué sucede con la salida de cat out.txt en el ejemplo. Luego repetirlo invertiendo el orden de las redirecciones. ¿Cambió algo?

- <p style="color:#096F93";>Respuesta:</p> *1* simboliza el file descriptor de stdout y *2* el de stderr,*>* nos marca la redireccion y *&** indica que lo que sigue y precede es un descriptor de archivo y no un nombre de archivo. Entonces la construcción se convierte en: 2>&1.

Ejecucion de los comandos:

```
$ ls -C /home /noexiste >out.txt 2>&1
	Program: [ls -C /home /noexiste >out.txt 2>&1] exited, status: 2 

$ cat out.txt
ls: cannot access '/noexiste': No such file or directory
/home:
lucaswaisten
	Program: [cat out.txt] exited, status: 0 
```
Invierto el orden:

```
 (/home/lucaswaisten) 
$ ls -C /home /noeiste >out.txt 1>&2
	Program: [ls -C /home /noeiste >out.txt 1>&2] exited, status: 2 
 (/home/lucaswaisten) 
$ cat out.txt
/home:
lucaswaisten
	Program: [cat out.txt] exited, status: 0 

```

---

### Tuberías simples (pipes)
- <p style="color:#096F93";>Pregunta:</p> Investigar qué ocurre con el exit code reportado por la shell si se ejecuta un pipe ¿Cambia en algo? ¿Qué ocurre si, en un pipe, alguno de los comandos falla? Mostrar evidencia (e.g. salidas de terminal) de este comportamiento usando bash. Comparar con la implementación del este lab.
- <p style="color:#096F93";>Respuesta:</p>El exit code reportado por la shell corresponde al ultimo comando que se ejecute. Tambien existe la opcion de pipefail, cuyo cayo reportara error si algunos de los comando fallo o cero en caso de exito.<br> Ejemplos en bash:
```
lucaswaisten@lucaswaisten-Sword-15-A11UD:~$ ls -l | grep no_existe
lucaswaisten@lucaswaisten-Sword-15-A11UD:~$ echo $?
1
```
Lo que se reporta consecuentemente es 1 ya que `ls -l` funciona devolviendo 0, pero `grep` no encuentra el directorio, devolviendo error
```
lucaswaisten@lucaswaisten-Sword-15-A11UD:~$ ls -l | grep no_existe | echo hola
hola
lucaswaisten@lucaswaisten-Sword-15-A11UD:~$ echo $?
0
```
<p>En este caso obtenemos en el exite code un 0 ya que <code>echo hola</code> no reporta error</p>
---

### Pseudo-variables
- <p style="color:#096F93";>Pregunta:</p> Investigar al menos otras tres variables mágicas estándar, y describir su propósito. Incluir un ejemplo de su uso en bash (u otra terminal similar).
- <p style="color:#096F93";>Respuesta:</p>
`$_`: The last argument of the previous command.
```
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ echo hola lucas
hola lucas
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ echo $_
lucas

```
`$!`: The process ID of the last background command.
```
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ echo hola &
[1] 30200
hola
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ echo hola
hola
[1]+  Done                    echo hola
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ echo $!
30200
```
`$$`: The process ID of the current shell.
```
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ echo $$
18030
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ ps -ef|grep 16487
lucaswa+   30905   18030  0 18:14 pts/1    00:00:00 grep --color=auto 16487
lucaswaisten@lucaswaisten-Sword-15-A11UD:$ ps -ef | grep `echo $$`
lucaswa+   18030   18022  0 14:55 pts/1    00:00:00 bash
lucaswa+   30914   18030  0 18:14 pts/1    00:00:00 ps -ef
lucaswa+   30915   18030  0 18:14 pts/1    00:00:00 grep --color=auto 18030

```
---

### Desafio:

#### ¿Cuál es la función de los parámetros MIN y TIME del modo no canónico? ¿Qué se logra en el ejemplo dado al establecer a MIN en 1 y a TIME en 0?

- `MIN` : indica la cantidad minima de bytes necesarios para que `read` retorne.
- `TIME` : indica cuanto tiempo debe esperar para retornar.

En el ejemplo, como `TIME == 0` la funcion `read` no necesita esperar, retorna tan pronto como encuentre 1 byte.

[source](https://www.gnu.org/software/libc/manual/html_node/Noncanonical-Input.html)


