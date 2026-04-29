# Configuration des Matériaux

Les **matériaux** définissent l’apparence des objets dans la scène. Chaque primitive référence un matériau via son `id`.  

Un matériau peut contrôler la couleur, la réflexion, la transparence, ou d’autres propriétés physiques.

---

## Exemple complet
```
materials: (
    { id = "gold"; type = "lambertian"; color = { r = 0.8; g = 0.6; b = 0.2; }; },
    { id = "glass"; type = "transparent"; color = { r = 1.0; g = 1.0; b = 1.0; }; ref = 1.5; },
    { id = "water"; type = "transparent"; color = { r = 0.8; g = 0.9; b = 1.0; }; ref = 1.33; },
    { id = "red_plastic"; type = "lambertian"; color = { r = 0.7; g = 0.1; b = 0.1; }; },
    { id = "ground"; type = "lambertian"; color = { r = 0.15; g = 0.15; b = 0.15; }; }
);
```

---

## 📊 Paramètres principaux des matériaux

| Paramètre      | Type      | Description |
|---------------|-----------|------------|
| `id`          | string    | Identifiant unique du matériau, utilisé par les primitives. |
| `type`        | string    | Type du matériau. Exemples : `lambertian` (mat), `reflective` (miroir), `transparent` (verre, eau). |
| `color`       | group     | Couleur principale du matériau, `{ r, g, b }`, valeurs de 0 à 1. |
| `ref`         | float     | Indice de réfraction pour les matériaux transparents. Exemple : verre ≈ 1.5, eau ≈ 1.33. |
---

## 🔹 Exemples unitaires simples

### 1. Matériau lambertian (diffus)
```yaml
{ id = "red_plastic"; type = "lambertian"; color = { r = 0.7; g = 0.1; b = 0.1; }; }
```
- Rouge, non réfléchissant, idéal pour plastique ou peinture.

### 2. Matériau transparent
```yaml
{ id = "glass"; type = "transparent"; color = { r = 1.0; g = 1.0; b = 1.0; }; ref = 1.5; }
```
- Transparence complète, simule le verre avec réfraction.

### 3. Matériau réfléchissant
```yaml
{ id = "mirror"; type = "reflective"; reflectivity = 0.9; color = { r = 1.0; g = 1.0; b = 1.0; }; }
```
- Très réfléchissant, comme un miroir.

### 4. Matériau pour le sol
```yaml
{ id = "ground"; type = "lambertian"; color = { r = 0.15; g = 0.15; b = 0.15; }; }
```
- Gris foncé, parfait pour un sol neutre.

---

## ✅ Astuces

- **Toujours donner un `id` unique** à chaque matériau.  
- Les valeurs `r, g, b` vont de **0.0 à 255.0** pour plus de précision.  
- Pour un matériau transparent, ajoute toujours `ref` (indice de réfraction).  

Ce système permet de combiner facilement plusieurs matériaux dans une scène, tout en gardant les primitives indépendantes.