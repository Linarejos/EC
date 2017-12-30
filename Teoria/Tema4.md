# Tema 3 | Segmentación de cauce

## Concepto de segmentación
Consiste en subdividir el proceso general en n etapas permitidendo el solapamiento en tiempo de ejecución de diferentes instrucciones. Cada instrucción se ejecuta en una fracción del tiempo T genral.

## Aceleración
Las **causas que la disminuyen** son:
  - Coste de la segmentación.
  - Duración del ciclo del reloj que viene impuesta por la etapa más lenta.
  - Riesgos de hazards: Bloquelo en el avance de instrucciones.

## Riesgos
Un riesgo es una situación que impide la ejecución de la siguiente instrucción del flujo de programa durante el ciclo de reloj designado, que bliga a modidicar la forma en la que avanzan las intrucciones hacia las etapas siguientes, que implica una educción de las prestaciones logradas por la segmientación.

De esta forma aparece:
  - **Riesgos estructurales**: dos intrucciones necesitan un mismo tipo de recurso por ejemplo acceder a memoria. Para solucionarlo se usa la precaptación, para reducir los fallos de caché.
  - **Riesgo por dependencias**: Acceso a datos cuyo valor viene actualizado por otra instrucción precedente. Las dependencias de datos las descubre el hardware al decoficar las isntrunciones. Alternativamente las podría reolver el compilador así el hardware sería mas simple. pero aumentaría el tamaño del código.
  - **Riesgos de control**: Consecuencia de las instrucciones de salto condicional. Debido a la captación de un salto no se conoce la dirección de salto. La solución es averiguarllo antes posoible si se va a saltar usando distitnas técnicas:
    - **Salto retardado**: El compilar pone delante del salto instrucciones que no le imfluencia para captar más.
    - **Annulling branch**: Un salto de este tipo ejecuta la(s) instrucción(es) siguiente(s) sólo si el salto se produce, pero la(s) ignora si el salto no se produce
    - **Prección de saltos**: Intentar predecir si una instrucción de salto concreta sará lugar a un salto o no. Existen dos tipos: Estatica (se toma la misma decisión para instrucción) y dinamica (cambia según la historia de eecución)
