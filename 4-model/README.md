# 3D Horus Model

![horus.gif](horus.gif)

## Normal Mapping

Técnica que simula detalles finos en la superficie de un objeto sin necesidad de aumentar su complejidad geométrica. Utiliza un mapa de normales (una textura donde cada píxel representa una dirección de normal) para alterar la dirección de iluminación a nivel de fragmento. Esto permite generar rugosidades, relieves y bordes con mayor realismo visual, manteniendo bajo el costo computacional.

## Mapa Especular

Define qué áreas de una superficie son más reflectantes o brillantes, simulando materiales como metal, plástico o cerámica. Cada píxel del mapa indica la intensidad con la que ese fragmento refleja la luz especular.

## Luz Direccional

Simula una fuente de luz lejana, como el sol, que proyecta rayos paralelos en una dirección fija. No se atenúa con la distancia.

## Luz Puntual

Emite luz desde una posición específica en todas las direcciones, como una bombilla. Su intensidad se atenúa con la distancia al fragmento mediante una función de caída (`falloff`).

## Modelo Horus

El modelo `Horus.obj` fue descargado de `CGTrader.com` (gratuito). El paquete incluye archivos `.obj`, `.mtl`, varios `.fbx`, y una carpeta `Textures` que contiene los mapas difuso y especular.

El archivo `Horus.obj` es cargado utilizando la biblioteca `Assimp`, que importa modelos 3D y los convierte en estructuras compatibles con OpenGL.

Las texturas se cargan con `stb_image` y se aplican a través de los shaders; `diffuseMap` determina el color base del modelo y `specularMap` controla el brillo especular.

## Instrucciones

### Instalación

```bash
brew install cmake
brew install glew
brew install glfw
brew install glm
brew install assimp
```

### Compilación

1. Crear un directorio de build:

```bash
mkdir build
cd build
```

2. Generar los archivos de build con CMake:

```bash
cmake ..
```

3. Compilar el proyecto:

```bash
make
```

### Ejecución

Una vez compilado, ejecutar el programa:
```bash
./MyProgram
```
