# Configuration du Ciel (Sky)

Le **ciel** définit l’environnement global derrière les objets et influence l’éclairage ambiant. Il peut être de différents types : galaxie, atmosphérique, ou vide.

---

## Exemple complet
```yaml
sky = {
type = "galaxy";
nebulaColor = { r = 0.0; g = 0.0; b = 1.0; };
};
```

---

## 📊 Paramètres principaux du ciel

| Paramètre           | Type      | Description |
|--------------------|-----------|------------|
| `type`             | string    | Type de ciel : `galaxy`, `atmospheric`, `empty`. |
| `nebulaColor`      | group     | Couleur principale de la nébuleuse pour les galaxies `{ r, g, b }`. |
| `starDensity`       | float     | Densité des étoiles pour un ciel de type galaxie (par défaut 0.001). |
| `nebulaDensity`     | float     | Densité de la nébuleuse (par défaut 0.7). |
| `nebulaContrast`    | float     | Contraste de la nébuleuse (par défaut 3.0). |
| `groundColor`       | group     | Couleur du sol pour le ciel atmosphérique `{ r, g, b }`. |
| `zenithColor`       | group     | Couleur du zénith pour le ciel atmosphérique `{ r, g, b }`. |

---

## 🔹 Exemples unitaires simples

### 1. Ciel Galaxie
```sky = {
type = "galaxy";
nebulaColor = { r = 0.0; g = 0.0; b = 1.0; };
};
```
- Crée un ciel rempli d’étoiles avec une nébuleuse bleue.  
- Paramètres internes : `starDensity = 0.001`, `nebulaDensity = 0.7`, `nebulaContrast = 3.0`.

### 2. Ciel Atmosphérique
```
sky = {
type = "atmospheric";
groundColor = { r = 0.5; g = 0.7; b = 1.0; };
zenithColor = { r = 1.0; g = 1.0; b = 1.0; };
};
```
- Simule un ciel terrestre avec un dégradé du sol au zénith.  

### 3. Ciel Vide
```
sky = {
type = "empty";
};
```

- Aucune couleur ni étoile. Idéal pour des tests ou scènes isolées.

---

## ✅ Astuces

- **Choisir le type de ciel selon la scène** : galaxie pour les scènes spatiales, atmospheric pour le réalisme terrestre, empty pour le minimalisme.  
- Les couleurs `{ r, g, b }` doivent être normalisées ou adaptées au moteur (souvent 0–1 ou 0–255 selon l’implémentation).  
- Les paramètres internes comme `starDensity` ou `nebulaContrast` peuvent être modifiés via les settings pour ajuster l’apparence sans changer le code de base.  

Cette configuration permet de définir l’arrière-plan et l’ambiance lumineuse globale, influençant directement la perception des objets et matériaux dans la scène.