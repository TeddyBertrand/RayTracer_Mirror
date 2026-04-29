# Configuration de la Caméra
Définit le point de vue de l'observateur. Une seule caméra par fichier.

```yaml
camera:
{
    type = "perspective";
    width = 1280;
    height = 720;
    position = { x = 0.0; y = 2.5; z = 8.0; };
    look_at = { x = 0.0; y = 0.0; z = 0.0; };
    up = { x = 0.0; y = 1.0; z = 0.0; };
    fieldOfView = 75.0;
};
```

**Paramètres de la caméra :**

| Paramètre    | Type   | Description                               | Par défaut        |
|-------------|--------|-------------------------------------------|-----------------|
| type        | string | Type de projection (ex: "perspective").   | Requis          |
| width       | int    | Largeur de l'image en pixels.             | 1920            |
| height      | int    | Hauteur de l'image en pixels.             | 1080            |
| position    | group  | Coordonnées {x, y, z} de l'origine.       | {x=0, y=0, z=0} |
| look_at     | group  | Point {x, y, z} ciblé par la caméra.     | {x=0, y=0, z=-1}|
| up          | group  | Vecteur {x, y, z} de l'axe vertical.     | {x=0, y=1, z=0} |
| fieldOfView | float  | Angle d'ouverture horizontal (°).        | 90.0            |
