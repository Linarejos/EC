# Tema 6 | Memoria

### Localidad de las referencias
  - **Localidad Espacial**: Si se referencia un elemento, los elementos cercanos serán referneciados pronto: los datos se almacenan juntos.
  - **Localidad Secuencial**: Las direcciones de memoria utilizadas suelen ser contiguas: instrucciones que se ejecutan son secuenciales.
  - **Localidad Temporal**: Si se rederencia un elemento, volverá a referenciarse pronto, la información se usará en un futuro proximo aproximadamente la misma que se está usando actualmente: bucles, repeticiones.

### Jerarquía de memoria
Teniendo en cuenta las limitaciones tecnológicas, y aprovechando el principio de localidad se puede conseguir un sistema de memoria eficaz mediante una jerarquía de niveles de memoria. Cada nivel tiene una capacidad menor pero es más rápido que los inferiores, toda la información de un nivel se encuentra en el nivel superior.

Registros -> Caché -> Memoria principal -> Disco sólido -> Unidades de cinta

Como los datos contenidos en los niveles superiores pueden no estar en los inferiores por ello cuando transferimos información lo hacemos en bloques completos en vez de transferir unicamente la palabra requerida.

Si una palabra es modificada en un nivel superior más tarde o más temprano tiene que escribirse en lo sniveles inferiores.

### Memorias semiconductoras
Son memorias de solo lectura y existen varios tipos:
  - **ROM** (Read Only Memory): La información se graba en el proceso de fabricación mediante máscaras.
  - **PROM** (Programmable ROM): La información se graba en un proceso posterior irreversible, programable una sola vez.
  - **EPROM** (Erasable Programmable ROM): Se pueden borrar exposiendolas a radiación ultravioleta, programable varias veces. Los datos permanecen de 10 a 100 años
  - **OTPROM** (One Time Programmable ROM): Como la EPROM sin ventana, más barata pero solo se programa una vez.
  - **EEPROM** (Electrically Erasable Programmable ROM): Se programa byte a byte mediante corrientes elevadas, reprogramable ne la misma placa.
  - **Memorias FLASH**: EEPROM que se puede reprogramar por bloques, mayor densidad y menor precio por byte.

Las memorias de lectura y escritura, también llamadas RAM (random access memory):
  - **SRAM** (Static Random Access Memory): los datos almacenados se mantienen por un tiempo indefinido si hay alimentación.
  - **DRAM** (Dynamic RAM): os datos almacenados decaen o se desvanecen y deben ser restaurados a intervalos regulares.


## Memoria Caché
Son memorias pequeñas de 8K a 6MB y rápidad, 5-10 veces más rapidas que la MP situada entre le procesador y la MP. El éxito de la memoria caché se debe a la propiedad de localidad de los programas, la información se va a utilizar en el futuro próximo probablemente ya se encuentran en caché por lo mencionado anteriormente.

Los objetivos en el diseño de un sistema con chaché es:
  1. Manimizar el índice de acientos
  2. Minimizar tiempo de acceso t.
  3. Minimizar el retardo por la falta.
  4. Minimizar la penalización por la actualización de la MP.

### Politicas u otros parametros.
Parámetro determinantes en el correcto funcionamiento y efectividad de un sistema caché:

  - **Politica de extracción**: Decide cuando y que información se va a introducir en la caché, el objetivo es máximizar los aciertos y se distinguen dos criterios de búsqueda básicos:
    - **Por demanda**: Cuando se necesita la información. (más sencillo)
    - **Prebusqueda**: Antes de necesitarla. (complejo y eficiente)
      - **Siempre**: siempre se busca el bloque i+1 siempre que se busque el i.
      - **Por falta**: siempre se busca el i+1 si se produce una falta en el bloque i.
      - **Marcada**: se busca el i+1 si se referencia a i por una falta o se referencia a i por primera vez.

  - **Politica de colocación**: La política de colocación determina la función de correspondencia entre bloques de la MP y marcos de bloque de la caché.
    - **Correspondencia directa**: Bloque i de MP -> marco de bloque i mod 2^n de caché. A cada marco de bloque le corresponde sólo un subconjunto de bloques de MP.
    - **Totalmente asociativa**: Un bloque de MP puede residir en cualquier marco de bloque de caché. Cuando se presenta una solicitud a la caché, todas las marcas se comparan simultáneamente para ver si el bloque está en la caché.
    - **Asociativa por conjuntos**: La caché se subdivide en 2^c conjuntos distintos, en 2^(n-c) marcos de bloque por conjunto. El bloque i de MP corresponde al conjunto i mod 2^c de caché. A este modo también se le llama asociativa por vias, la via i está formada por todos los marcos que ocupan el lugar i-esimo dentro de su conjunto.
    - **Correspondencia por sectores**: La MP se divide en 2^s grupos disjuntos llamados sectores -> 2^(N-s) bloques por sector. La caché se compone pues de 2^(n-(N-s)) marcos de sectores cada uno con 2^(N-s) marcos de bloque. Un sector de la MP puede estar en cualquier marco de sector, pero la correspondencia de bloques dentro de un sector es directa. Se lleva a caché el bloque que provocó la falta, los restantes marcos se marcan como no validos al cargar el nuevo sector.

  - **Politica de reemplazo**: Cuando se produce una falta y la caché está llena, saber los bloques que han de sustituirse. Existen diferentes algoritmos que son:
    - **FIFO**:  De todos los bloques existentes en caché se reemplaza el primero que se introdujo.
    - **LRU**: El más usado, se reemplaza el menos recientemente referenciado.
    - **RAND**: Se elige aleatoriamente el bloque.

  - **Politica de actualización de la MP**: ¿Cuando debe modificarse el contenido de la MP tras escribir en caché?


























  -
