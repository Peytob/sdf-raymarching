# SDF-raymarching

Экспериментальный проект, целью которого является реализация загрузки SDF-сцен из универсального JSON представления и последующий рендеринг посредствам обработки сохраненного в SSBO дерева.

## Сборка

Установите зависимости проекта, чтобы Cmake мог их найти (все зависимости явно и красиво выделены комментариями в CMakeLists):

- [Nlohmann json](https://github.com/nlohmann/json)
- OpenGL 4
- [GLFW](https://github.com/glfw/glfw)
- [GLEW](https://github.com/nigels-com/glew)
- [GLM](https://github.com/g-truc/glm)

Сгенерируйте файлы сборки при помощи Cmake и используйте вашу систему сборки.

```bash
mkdir build
cmake -G "MinGW Makefiles" -S . -B ./build
cd ./build
make
```

Протестирована сборка на Windows с помощью mingw32.

## Способы отрисовки сцены

Поддерживается 2 формата отрисовки сцены:

1) Посредствам сохранения дерева, описывающего сцену, в SSBO. Данный формат имеет **очень низкую** производительность, всвязи с обходом дерева Post-order (левое поддерево - правое поддерево - текущий узел). Обход дерева реализован посредствам работы со стеком, что порождает вложенные циклы while, которые с трудом пережевывает видеокарта.

2) Посредствам кодогенерации - генерируется GLSL метод, который используется для отрисовки сцены и помещается как часть реализации шейдерной программы. Работает значительно быстрее.

## Формат сохранения сцены

Сцена сохраняется в JSON документ следующего формата (todo проверить правильность json-schema). Готовый пример можно посмотреть в [примере сцены](https://github.com/Peytob/sdf-raymarching/blob/main/resources/example_scene.json):

***Схемы узлов дерева сцены***

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "OperationNode",
    "type": "object",
    "properties": {
        "operation": {
            "type": "string",
            "enum": ["merge", "subtraction", "intersection"]
        },
        "left": {
            "anyOf": [
                { "$ref": "#" },
                { "$ref": "LeafNode" }
            ]
        },
        "right": {
            "anyOf": [
                { "$ref": "#" },
                { "$ref": "LeafNode" }
            ]
        }
    },
    "required": ["operation", "left", "right"]
}
```

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "LeafNode",
    "type": "object",
    "properties": {
        "operation": {
            "type": "string",
            "const": "leaf"
        },
        "materialId": {
            "type": "string"
        },
        "figure": {
            "anyOf": [
                { "#ref": "Sphere" },
                { "#ref": "Box" },
                { "#ref": "Torus" },
                { "#ref": "Plane" },
                { "#ref": "Cylinder" },
            ]
        }
    },
    "required": ["operation", "materialId", "figure"]
}
```

***Схемы фигур***

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Sphere",
    "type": "object",
    "properties": {
        "type": {
            "type": "string",
            "const": "sphere"
        },
        "radius": {
            "type": "number"
        }
    },
    "required": ["type", "radius"]
}
```

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Box",
    "type": "object",
    "properties": {
        "type": {
            "type": "string"
        },
        "sizes": {
            "type": "array",
            "items": [
                { "type": "number", "comment": "X axis size" },
                { "type": "number", "comment": "Y axis size" },
                { "type": "number", "comment": "Z axis size" }
            ]
        }
    },
    "required": ["type", "sizes"]
}
```

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Torus",
    "type": "object",
    "properties": {
        "type": {
            "type": "string",
            "const": "torus"
        },
        "largeRadius": {
            "type": "number"
        },
        "smallRadius": {
            "type": "number"
        }
    },
    "required": ["type", "largeRadius", "smallRadius"]
}
```

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Plane",
    "type": "object",
    "properties": {
        "type": {
            "type": "string",
            "const": "plane"
        },
        "normal": {
            "type": "array",
            "items": [
                { "type": "number", "comment": "X" },
                { "type": "number", "comment": "Y" },
                { "type": "number", "comment": "Z" }
            ]
        },
        "distanceFromOrigin": {
            "type": "number"
        }
    },
    "required": ["type", "normal", "distanceFromOrigin"]
}
```

```json
{
    "$schema": "http://json-schema.org/draft-07/schema#",
    "title": "Cylinder",
    "type": "object",
    "properties": {
        "type": {
            "type": "string",
            "const": "cylinder"
        },
        "radius": {
            "type": "number"
        },
        "height": {
            "type": "number"
        }
    },
    "required": ["type", "radius", "height"]
}
```

## Возможно, когда-нибудь будет реализовано

- [x] Конфигурация приложения из конфигурационного файла
- [ ] Указание источников освещения в JSON файле сцены
- [ ] Ambient Occlusion
- [ ] Soft Shadows

## Скриншот

Я проявил всю свою творческую жилку на эту замечательную демонстрационную сцену.

![Scene](/images/example_scene.png)
