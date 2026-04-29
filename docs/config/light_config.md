# Configuration des Lumières

Les **lumières** définissent l’éclairage de la scène. Chaque lumière influence la manière dont les matériaux et objets apparaissent, en jouant sur la couleur, l’intensité et la position.

---

## Exemple complet
```
lights: (
{
type = "point"
position = { x = 10.0; y = 5.0; z = 0.0; };
color = { r = 255.0; g = 0.0; b = 0.0; };
intensity = 1.3
},
{
type = "point"
position = { x = -10.0; y = 5.0; z = 0.0; };
color = { r = 0.0; g = 255.0; b = 0.0; };
intensity = 1.3
}
)
```

---

## 📊 Paramètres principaux des lumières

| Paramètre      | Type      | Description |
|---------------|-----------|------------|
| `type`        | string    | Type de lumière. Exemples : `point`, `directional`, `spot`. |
| `position`    | group     | Position de la lumière dans l’espace `{ x, y, z }`. Obligatoire pour les lumières ponctuelles (`point`). |
| `color`       | group     | Couleur de la lumière `{ r, g, b }`, valeurs de 0 à 255. |
| `intensity`   | float     | Intensité lumineuse. Valeur ≥ 0, influence la luminosité de la lumière. |

---

## 🔹 Exemples unitaires simples

### 1. Lumière ponctuelle rouge
```
{
type = "point"
position = { x = 10.0; y = 5.0; z = 0.0; };
color = { r = 255.0; g = 0.0; b = 0.0; };
intensity = 1.3
}
```

- Éclaire la scène depuis la droite avec une teinte rouge.

### 2. Lumière ponctuelle verte
```
{
type = "point"
position = { x = -10.0; y = 5.0; z = 0.0; };
color = { r = 0.0; g = 255.0; b = 0.0; };
intensity = 1.3
}
```

- Éclaire la scène depuis la gauche avec une teinte verte.

---

## ✅ Astuces

- **Positionnez vos lumières judicieusement** pour créer des ombres et des contrastes intéressants.  
- Les valeurs `r, g, b` vont de **0.0 à 255.0**, contrairement aux matériaux qui peuvent utiliser 0–1.  
- Ajustez `intensity` pour obtenir des effets plus doux ou plus dramatiques.  
- Vous pouvez combiner plusieurs types de lumières pour enrichir votre scène (ex. `point` + `directional`).  

Cette configuration permet de contrôler précisément l’éclairage et l’ambiance de votre scène 3D.
