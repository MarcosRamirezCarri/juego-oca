# Documentación Técnica - Juego de la Oca

## Diseño Orientado a Objetos

### 1. Principios POO Aplicados

#### 1.1 Encapsulación
La encapsulación se aplica de manera consistente en todo el proyecto. Cada clase mantiene sus datos privados y proporciona una interfaz pública controlada para acceder y modificar su estado. Por ejemplo, la clase `Jugador` mantiene privados su nombre, posición, turnos perdidos y estado del pozo, pero ofrece métodos públicos como `conseguirNombre()`, `moverJugador()` y `puedeJugar()` que permiten una manipulación segura de estos datos.

Esta implementación de encapsulación no solo protege la integridad de los datos, sino que también facilita el mantenimiento del código. Si en el futuro necesitamos cambiar la forma en que se almacena la posición del jugador o agregar validaciones adicionales, solo necesitamos modificar los métodos públicos sin afectar el resto del código que utiliza la clase.

#### 1.2 Herencia
La herencia se utiliza para crear una jerarquía de casillas especiales que comparten comportamiento común pero implementan efectos específicos. La clase `Casilla` actúa como clase base abstracta, definiendo la interfaz común que todas las casillas deben implementar a través del método virtual `accionJugador()`.

Esta estructura de herencia permite que cada tipo de casilla (Oca, Puente, Pozo, etc.) implemente su propio comportamiento específico mientras mantiene la misma interfaz. Por ejemplo, una `CasillaOca` salta al jugador a la siguiente oca y otorga un turno extra, mientras que una `CasillaPozo` atrapa al jugador hasta que otro caiga en la misma casilla. Esta flexibilidad hace que sea muy fácil agregar nuevos tipos de casillas en el futuro sin modificar el código existente.

#### 1.3 Polimorfismo
El polimorfismo se manifiesta en la capacidad de tratar todas las casillas de manera uniforme a través de la interfaz común definida en la clase base. Cuando un jugador cae en una casilla, el código simplemente llama a `casilla->accionJugador(jugador)`, y el comportamiento específico se determina en tiempo de ejecución según el tipo real de la casilla.

Este enfoque polimórfico elimina la necesidad de usar múltiples declaraciones `if-else` o `switch` para determinar qué hacer según el tipo de casilla. En su lugar, cada subclase implementa su propia lógica, y el sistema de enlace dinámico de C++ se encarga de llamar al método correcto. Esto hace que el código sea más limpio, más fácil de mantener y más extensible.

#### 1.4 Composición
La composición se utiliza para construir objetos complejos a partir de objetos más simples. La clase `Juego` contiene instancias de `Jugador`, `Dado` y un vector de casillas, mientras que `JuegoGUI` contiene una instancia de `Juego` y maneja la interfaz gráfica. Esta estructura refleja las relaciones naturales del dominio del problema: un juego tiene jugadores, un dado y casillas, y una interfaz gráfica tiene un juego.

La composición se implementa usando smart pointers (`std::unique_ptr`) para gestionar automáticamente el ciclo de vida de los objetos compuestos. Esto garantiza que cuando se destruye un objeto `JuegoGUI`, todos sus componentes se liberan automáticamente, previniendo memory leaks y simplificando la gestión de memoria.

### 3. Análisis de Clases

#### 3.1 Clase Juego
La clase `Juego` actúa como el orquestador principal del sistema, coordinando todas las interacciones entre los diferentes componentes. Su responsabilidad principal es controlar el flujo del juego, gestionando los turnos de los jugadores, aplicando las reglas del juego y verificando las condiciones de victoria.

Esta clase mantiene el estado global del juego a través de varios atributos clave. El vector `jugadores` almacena todas las instancias de jugadores participantes, permitiendo un número variable de jugadores (típicamente entre 2 y 4). El smart pointer `dado` y el vector `casillas` representan los componentes fundamentales del juego, gestionando automáticamente su ciclo de vida. El `jugadorActual` mantiene un seguimiento del turno actual, mientras que `finDelJuego` controla el estado de terminación del juego.

La implementación de esta clase demuestra un buen uso de la composición, ya que `Juego` no hereda de otras clases sino que utiliza instancias de `Jugador`, `Dado` y casillas para construir su funcionalidad. Esto facilita la reutilización de componentes y hace que el código sea más modular y fácil de probar.

#### 3.2 Clase Jugador
La clase `Jugador` encapsula toda la información y comportamiento relacionado con un participante individual del juego. Su diseño refleja el principio de responsabilidad única, ya que se enfoca exclusivamente en mantener el estado de un jugador y gestionar sus interacciones con el tablero.

Cada instancia de `Jugador` mantiene su propio estado a través de atributos bien definidos. El `nombre` permite identificar al jugador, mientras que la `posicion` rastrea su ubicación actual en el tablero. Los `turnosPerdidos` implementan el mecanismo de penalización del juego, permitiendo que ciertas casillas especiales afecten la capacidad del jugador para participar en turnos futuros. El flag `enPozo` maneja el caso especial de la casilla del pozo, donde un jugador puede quedar atrapado hasta que otro caiga en la misma casilla.

Esta implementación demuestra una buena aplicación de encapsulación, ya que todos los atributos son privados y solo se puede acceder a ellos a través de métodos públicos bien definidos. Por ejemplo, en lugar de permitir acceso directo a `posicion`, se proporciona el método `conseguirPosicion()` que puede incluir validaciones adicionales si es necesario en el futuro.

#### 3.3 Clase Casilla (Abstracta)
La clase `Casilla` representa el ejemplo más claro de abstracción en el proyecto, definiendo una interfaz común que todas las casillas especiales deben implementar. Al ser una clase abstracta, no se pueden crear instancias directas de `Casilla`, pero proporciona el contrato que todas las subclases deben cumplir.

Esta clase define dos métodos virtuales fundamentales que encapsulan el comportamiento esencial de cualquier casilla. El método `accionJugador(Jugador&)` permite la polimórfia, ya que cada subclase implementa su propia lógica específica para afectar al jugador que cae en esa casilla. El método `getDescripcion()` proporciona información textual sobre el efecto de la casilla, ademas de permitir la polimórfia en este mismo.

La elección de hacer `accionJugador()` virtual puro (abstracto) fuerza a todas las subclases a implementar este comportamiento, garantizando que no se pueda crear una casilla sin definir su efecto. Esto previene errores de diseño y asegura que el sistema sea completo y coherente.

#### 3.4 Subclases de Casilla
Las subclases de `Casilla` implementan la variedad de efectos especiales que hacen del Juego de la Oca una experiencia dinámica e impredecible. Cada subclase hereda de la clase base y proporciona una implementación única del método `accionJugador()`, demostrando el poder del polimorfismo en la práctica.

La `CasillaOca` implementa uno de los efectos más característicos del juego. Cuando un jugador cae en una oca, no solo salta automáticamente a la siguiente casilla de oca, sino que también recibe un turno extra. Esta implementación requiere conocimiento de la estructura del tablero para calcular la siguiente posición de oca, demostrando cómo las subclases pueden acceder a información del contexto del juego.

La `CasillaPuente` proporciona un efecto de transporte directo, moviendo al jugador a una posición específica (casilla 12) sin pasar por las casillas intermedias. Este tipo de efecto es común en juegos de mesa y demuestra cómo el polimorfismo permite implementar comportamientos complejos de manera simple y elegante.

Las casillas de penalización como `CasillaPosada` y `CasillaCarcel` implementan el mecanismo de pérdida de turnos, afectando la capacidad del jugador para participar en futuras rondas. La diferencia en la duración de la penalización (1 vs 2 turnos) se maneja simplemente pasando diferentes valores al método `perderTurnos()` del jugador.

La `CasillaPozo` implementa uno de los efectos más complejos del juego, ya que requiere coordinación entre múltiples jugadores. Cuando un jugador cae en el pozo, queda atrapado hasta que otro jugador caiga en la misma casilla. Esta implementación demuestra cómo las subclases pueden modificar el estado de otros objetos en el sistema.

La `CasillaLaberinto` y `CasillaCalavera` implementan efectos de retroceso, moviendo al jugador a posiciones anteriores en el tablero. Estos efectos añaden un elemento de frustración y estrategia al juego, ya que pueden hacer que un jugador que estaba cerca de la meta tenga que volver a empezar.

Finalmente, la `CasillaJardin` representa la meta del juego, verificando si el jugador ha llegado exactamente a la posición 63 para determinar si ha ganado. Esta casilla no aplica efectos especiales al jugador, sino que actúa como un punto de verificación para el estado del juego.

### 4. Patrones de Diseño

El patrón Strategy se implementa de manera natural a través del sistema de herencia y polimorfismo de las casillas. Cada tipo de casilla representa una estrategia diferente para afectar al jugador que cae en ella. Cuando un jugador se mueve a una nueva posición, el sistema simplemente llama a `accionJugador()` en la casilla correspondiente, y la estrategia específica se ejecuta automáticamente.

Esta implementación del Strategy Pattern es particularmente elegante porque elimina la necesidad de usar declaraciones condicionales complejas. En lugar de tener un método largo con múltiples `if-else` para determinar qué hacer según el tipo de casilla, cada casilla encapsula su propia lógica. Esto hace que el código sea más mantenible y extensible, ya que agregar un nuevo tipo de casilla solo requiere crear una nueva subclase sin modificar el código existente.


#### 4.2 Patron Observador (simplificado)
El Patron observador se implementa de manera simplificada en el mecanismo del pozo. Cuando un jugador cae en el pozo, todos los otros jugadores que estaban atrapados son notificados y liberados automáticamente. Aunque no se usa una implementación formal del patrón Observer con interfaces y listas de observadores, la funcionalidad es equivalente.

Esta implementación simplificada es apropiada para este contexto porque la relación de observación es simple y directa: solo hay un tipo de evento (alguien cae en el pozo) y una acción correspondiente (liberar a todos los jugadores atrapados). En un sistema más complejo, se podría implementar un sistema de eventos más sofisticado, pero para este proyecto la implementación actual es suficiente y mantiene el código simple y comprensible.

### 5. Gestión de Memoria

La gestión de memoria en este proyecto utiliza punteros inteligentes modernos de C++ para garantizar la seguridad y eficiencia.

Los punteros inteligentes se utilizan en dos contextos principales: para gestionar el dado en la clase `Juego`, y para gestionar las casillas individuales en el vector de casillas. Esta implementación garantiza que cuando un objeto `Juego` se destruye, todos sus componentes se liberan automáticamente, previniendo fugas de memoria que son comunes en proyectos que usan gestión manual de memoria.

Además, el uso de smart pointers proporciona seguridad garantizada. Si durante la construcción de un objeto se lanza una excepción, los punteros inteligentes ya creados se liberarán automáticamente, evitando fugas de memoria que podrían ocurrir con gestión manual de memoria.

### 6. Características de C++ Moderno

#### 6.1 C++17 Features

La deducción de tipos con `auto` se utiliza extensivamente en range-based for loops, eliminando la necesidad de especificar tipos explícitamente y haciendo el código más mantenible. Si en el futuro se cambia el tipo de contenedor, el código que usa `auto` seguirá funcionando sin modificaciones.

La inicialización de listas se utiliza para crear vectores y otros contenedores de manera más concisa y legible. Los range-based for loops proporcionan una forma más intuitiva de iterar sobre contenedores, eliminando la posibilidad de errores de índices y haciendo el código más expresivo.

Finalmente, el proyecto utiliza `std::string` en lugar de C-style strings, aprovechando la gestión automática de memoria y las funciones de utilidad proporcionadas por la biblioteca estándar.

### 7. Interfaz de Usuario

#### 7.1 Interfaz Gráfica con SFML
La interfaz gráfica del juego está implementada usando la biblioteca SFML (Simple and Fast Multimedia Library). La interfaz presenta el tablero de juego de manera visual, con representaciones gráficas de las casillas, jugadores y elementos del juego.

La clase `JuegoGUI` actúa como el controlador principal de la interfaz, manejando la renderización, eventos de usuario y la comunicación con la lógica del juego. Esta separación entre la lógica del juego y la interfaz gráfica sigue el patrón Model-View-Controller (MVC), facilitando el mantenimiento y la extensión del código.

La interfaz incluye elementos visuales como tokens de jugadores, representación gráfica del tablero, botones interactivos y un historial de acciones. Los jugadores pueden ver claramente su posición en el tablero, el estado del juego y las acciones disponibles, haciendo que la experiencia sea más inmersiva y fácil de seguir.

Los indicadores visuales, como los tokens de colores para cada jugador y las etiquetas de posición, proporcionan información contextual de manera inmediata y clara. La confirmación de acciones importantes, como el lanzamiento del dado o el movimiento a una nueva casilla, se muestra visualmente, ayudando al usuario a mantener el control sobre el juego y entender las consecuencias de sus acciones.

### 8. Extensibilidad

#### 8.1 Nuevas Casillas
La arquitectura del proyecto está diseñada para facilitar la extensión con nuevos tipos de casillas. El sistema de herencia y polimorfismo permite agregar nuevas casillas especiales sin modificar el código existente, siguiendo el principio Open/Closed de SOLID (abierto para extensión, cerrado para modificación).

Para agregar una nueva casilla, solo se requiere crear una nueva subclase que herede de `Casilla` e implemente el método `accionJugador()`. El resto del sistema automáticamente reconocerá y utilizará la nueva casilla sin necesidad de cambios adicionales. Esta flexibilidad es especialmente valiosa en un contexto educativo, donde los estudiantes pueden experimentar con diferentes tipos de casillas para entender mejor los conceptos de POO.

#### 8.2 Nuevas Reglas
La clase `Juego` está diseñada para ser extensible, permitiendo la implementación de nuevas mecánicas y reglas sin afectar la funcionalidad existente. La separación clara de responsabilidades entre las diferentes clases facilita la modificación de comportamientos específicos sin impactar otras partes del sistema.

La clase `Jugador` también está preparada para extensiones, con métodos que pueden ser fácilmente modificados o extendidos para soportar nuevos estados o comportamientos. Por ejemplo, se podría agregar un sistema de puntos, efectos temporales, o capacidades especiales sin necesidad de reescribir la lógica fundamental del juego.

#### 8.3 Interfaz Gráfica
La separación entre la lógica del juego y la interfaz de usuario es una característica importante del diseño. Las clases del modelo (Juego, Jugador, Casilla, etc.) son completamente independientes de la interfaz gráfica, lo que permite modificar o extender la interfaz sin afectar la lógica del juego.

Esta separación se demuestra en la implementación de la interfaz gráfica con SFML, que utiliza las mismas clases del modelo. La clase `JuegoGUI` actúa como una capa de presentación que se comunica con la lógica del juego a través de una interfaz bien definida, demostrando la robustez del diseño arquitectónico y la aplicabilidad del patrón MVC.