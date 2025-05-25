# Custom Look At

![camera.gif](camera.gif)

En gráficos 3D, la cámara no existe físicamente como un objeto: es una matriz de transformación que determina desde dónde y hacia dónde se observa la escena. Para construir esta matriz, se utiliza una función llamada `LookAt`.

La función `LookAt(eye, center, up)` genera una `view matrix` que transforma todo el mundo 3D al `view space`. En otras palabras, reposiciona y reorienta todos los objetos como si estuviéramos mirando desde una cámara virtual ubicada en eye, mirando hacia center, y con una orientación vertical dada por el vector up.

Internamente, `LookAt` construye un sistema de coordenadas local partir de tres vectores:

- `f (forward)`: dirección desde `eye` hacia `center`
- `r (right)`: vector perpendicular a `f` y `up`, que apunta hacia la derecha
- `u (up modificado)`: vector perpendicular a `f` y `r`, que completa la base ortonormal

Con estos vectores se forma una matriz de rotación, y luego se aplica una traslación inversa para colocar la escena en el sistema de referencia de la cámara.

## Problema

En este proyecto implementamos manualmente la función `LookAt` para entender cómo se construye una cámara en `OpenGL` sin depender de utilidades externas como `glm::lookAt`. Esto permite hacer el modelo de iluminación `Phong-Blinn` en espacio de view, en lugar de en espacio de mundo.

## Instrucciones

### Instalación

```bash
brew install cmake
brew install glew
brew install glfw
brew install glm
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
