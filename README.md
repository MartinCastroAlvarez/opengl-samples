# opengl-samples

Examples using OpenGL

# Referencias

- [Notas de Clase](https://github.com/vany-viu/03_60GIIN)

# Instrucciones

Para cada apartado de esta actividad desarrollar un proyecto nuevo a partir de la estructura del motor desarrollado en clase. En dicho proyecto, habrá un fichero `main.cpp` con el código necesario para resolver el problema, junto a ese fichero estarán aquellos *shaders* que sean necesarios para la resolución. El alumno es libre de realizar todas las modificaciones/ampliaciones que considere necesarias en el motor desarrollado en clase. Si se necesitara algún *asset* extra para alguna actividad se pueden dejar los ficheros necesarios en la carpeta `assets` a tal efecto en la estructura del proyecto.

Para la entrega se recomienda realizar un `tag` sobre el repositorio en *GitHub* del alumno, y en *Blackboard* pegar el enlace a dicho `tag` como entrega en la actividad. La fecha de dicho `tag` deberá ser anterior a la fecha límite de entrega de la actividad. Si el alumno no ha usado *GitHub* como se recomendaba, puede adjuntar a la entrega un fichero comprimido con toda la estructura del proyecto, código, assets,... que sean necesarios para su compilación y ejecución.

* [01](./1-light) - Hacer el modelo de iluminación Phong-Blinn en espacio de view, en lugar de en espacio de mundo.
* [02](./2-camera/) - En la cámara, implementar la función LookAt, y no usar la de GLM.
* [03](./3-lantern/) - Implementar un nuevo tipo de luz, FlashLight o Linterna, que funcione como una SpotLight (conos suavizados), pero que se mueva con la cámara y siempre ilumine hacia adelante. Como si el personaje de un juego llevara una linterna siempre apuntando hacia adelante.
* [04](./4-model/) - Importar un modelo 3D complejo a elección (de algún repositorio de modelos 3D gratis). El modelo tiene que tener lo necesario para pintarlo correctamente. Para pintar hay que usar usando `normal mapping` con el modelo de iluminación `Blinn`, mapas difusos y `speculares`, y usar al menos una luz direccional, y una point light.
* [05](./5-scene/) - Usando una escena 3D cualquiera, pintarla de manera normal desde el punto de vista de una cámara. Pero poner un panel en una esquina de la escena mostrando la misma escena desde el punto de vista de otra cámara fija, usando un `framebuffer` para ello.

