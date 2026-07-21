# Airport Routing System

Proyecto desarrollado para la asignatura Análisis y Diseño de Algoritmos II (AyDA 2) de Ingeniería en Sistemas (UNICEN).

## Descripción

Aplicación desarrollada en C++ para modelar una red de aeropuertos y vuelos utilizando estructuras de datos y grafos.

## Funcionalidades

- Gestión de aeropuertos.
- Gestión de vuelos.
- Consultas sobre la red.
- Procesamiento de reservas.
- Algoritmos sobre grafos.

## Tecnologías

- C++
- Programación Orientada a Objetos
- Grafos

## Aprendizajes

Durante este proyecto trabajé con estructuras de datos, modelado orientado a objetos y algoritmos aplicados sobre grafos.

## Mi rol en el proyecto

Este proyecto fue desarrollado de forma colaborativa junto a un compañero durante la cursada.

Mi participación incluyó la mayor parte del desarrollo de la aplicación, abarcando:

- Análisis de los requisitos.
- Diseño de la arquitectura de la solución.
- Implementación de las principales funcionalidades.
- Desarrollo de las estructuras de datos y algoritmos utilizados.
- Integración de los distintos módulos.
- Pruebas, depuración y corrección de errores.

## Sobre el Proyecto

##Introducción
El presente trabajo consiste en el desarrollo de una aplicación destinada a la administración de aeropuertos y la gestión de vuelos entre ellos. La aplicación deberá permitir la interacción con dos tipos principales de usuarios: por un lado, los viajeros, quienes podrán consultar información relacionada con vuelos disponibles y realizar operaciones vinculadas a sus viajes (servicios 1 y 2); y por otro lado, los organismos de seguridad, cuyo objetivo será supervisar el correcto funcionamiento de los aeropuertos mediante herramientas específicas de monitoreo (servicio 3).

Además, el sistema deberá incorporar funcionalidades para la consulta y listado de los aeropuertos registrados, así como también para la visualización de las reservas realizadas dentro de la plataforma.

##Sobre cada uno de los Servicios:
-Servicio 1: Este servicio permite verificar la existencia de vuelos directos (sin escalas) entre un aeropuerto de origen y un aeropuerto de destino determinados. En caso de encontrar un vuelo disponible que cumpla con dichas condiciones, el sistema proporcionará información relevante del mismo, como la distancia recorrida en kilómetros y la cantidad de asientos disponibles.

-Servicio 2: Este servicio permite obtener todos los vuelos disponibles entre un aeropuerto de origen y un aeropuerto de destino determinados, contemplando tanto rutas directas como aquellas que incluyen una o más escalas. Para cada ruta encontrada, el sistema brindará información relevante, como la aerolínea responsable del vuelo (la cual deberá mantenerse constante a lo largo de todas las escalas que conformen la ruta), la cantidad de escalas realizadas y la distancia total recorrida en kilómetros.

-Servicio 3: Este servicio proporciona un circuito de vuelos que permite recorrer todos los aeropuertos involucrados visitando cada uno de ellos una única vez y regresando finalmente al aeropuerto de origen. El circuito seleccionado deberá ser aquel que minimice la distancia total recorrida.

Para la ruta obtenida, el servicio brindará información sobre la distancia total en kilómetros, la cantidad de aeropuertos visitados y el detalle del recorrido realizado, indicando el nombre de cada aeropuerto en el orden en que fueron transitados.

##Modelado del problema
##Sistema de vuelos

Tipo de dato:
Se modela mediante un grafo no orientado rotulado, donde los vértices representan los aeropuertos y los arcos representan los vuelos disponibles entre ellos. Cada arco posee como rótulo un objeto de tipo Vuelo, que contiene la información asociada al vuelo.

Implementación:
El grafo se implementó utilizando la clase template Grafo desarrollada durante la cursada. Internamente utiliza un map de STL, donde cada clave representa el identificador único de un aeropuerto y su valor contiene la lista de vuelos disponibles desde dicho aeropuerto.

Ventajas y desventajas:
Esta estructura permite una representación eficiente de las conexiones entre aeropuertos, con una complejidad espacial de O(n + e), donde n es la cantidad de aeropuertos y e la cantidad de vuelos. Además, permite realizar búsquedas e inserciones de vértices de manera eficiente.

##Sistema de aeropuertos

Tipo de dato:
Se utiliza un arreglo dinámico donde cada posición almacena un objeto de tipo Aeropuerto. La posición dentro del arreglo coincide con el identificador único del aeropuerto dentro del grafo.

Implementación:
Fue implementado mediante un vector de STL, permitiendo acceder directamente a un aeropuerto a partir de su identificador.

Ventajas y desventajas:
La principal ventaja es el acceso directo a los elementos con complejidad O(1), lo que resulta útil para las consultas frecuentes del sistema. Además, la coincidencia entre la posición del vector y la clave del grafo evita búsquedas adicionales.
Como desventaja, al agregar elementos puede ser necesario redimensionar el vector, generando una copia interna del mismo.

##Sistema de reservas

Tipo de dato:
Se utiliza una lista STL de objetos Vuelo que contiene únicamente aquellos vuelos que poseen reservas.

Implementación:
Cada vuelo mantiene internamente la información de sus reservas, mientras que la lista externa permite acceder rápidamente a los vuelos con reservas realizadas.

Ventajas y desventajas:
Esta decisión evita recorrer todo el grafo para encontrar vuelos con reservas, mejorando la eficiencia del listado de reservas. Como desventaja, implica un mayor consumo de memoria al mantener información adicional.

##Aeropuerto

Tipo de dato:
Se implementó una clase propia para representar un aeropuerto.

Implementación:
Cada objeto Aeropuerto almacena:

Identificador único.
Nombre.
Ciudad.
País.
Lista de aerolíneas disponibles.

Además, posee métodos para acceder a dicha información.

##Vuelo

Tipo de dato:
Se implementó una clase propia para representar un vuelo.

Implementación:
Cada objeto Vuelo almacena:

Aeropuerto de origen y destino.
Aerolínea.
Distancia recorrida.
Cantidad de asientos totales.
Cantidad de reservas realizadas.
Información sobre si es un vuelo de cabotaje.

Ventajas y desventajas:
Al contener directamente la información de sus reservas y asientos disponibles, permite obtener estos datos de manera eficiente sin realizar búsquedas adicionales en otras estructuras.

##Conclusiones del proyecto

El desarrollo de este proyecto permitió aplicar conceptos de estructuras de datos y algoritmos para resolver un problema basado en un escenario real: la gestión de aeropuertos y rutas aéreas.

La elección de un grafo como estructura principal permitió modelar de manera natural la relación entre aeropuertos y vuelos, representando los aeropuertos como vértices y las conexiones aéreas como aristas. A partir de este modelo, fue posible implementar funcionalidades orientadas a la búsqueda, análisis y optimización de rutas, utilizando algoritmos propios de esta estructura de datos.

Más allá de la implementación técnica, este proyecto permitió comprender la importancia de seleccionar estructuras adecuadas para representar problemas del mundo real, evaluando aspectos como eficiencia, complejidad temporal y consumo de memoria.

Este trabajo fortaleció conocimientos en diseño de software, estructuras de datos, análisis de algoritmos y resolución de problemas, aplicando herramientas fundamentales para el desarrollo de soluciones informáticas eficientes y escalables.
