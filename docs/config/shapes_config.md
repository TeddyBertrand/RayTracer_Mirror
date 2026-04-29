# Configuration des Formes (Shapes)

Les **formes** représentent les objets géométriques dans la scène. Chaque forme peut avoir un matériau assigné qui détermine son apparence et ses propriétés physiques.

---

## Exemple complet
```
shapes: (
{ type = "sphere"; position = { x = 0.0; y = -1000.0; z = 0.0; }; radius = 1000.0; material = "ground"; },
{ type = "sphere"; position = { x = 0.0; y = 1.5; z = 0.0; }; radius = 1.5; material = "glass"; },
{ type = "sphere"; position = { x = 0.0; y = 1.5; z = 0.0; }; radius = -1.4; material = "glass"; },
{ type = "sphere"; position = { x = -4.0; y = 1.0; z = -2.0; }; radius = 1.0; material = "gold"; },
{ type = "sphere"; position = { x = 4.0; y = 1.0; z = -2.0; }; radius = 1.0; material = "red_plastic"; },
{ type = "sphere"; position = { x = -2.5; y = 0.7; z = 3.0; }; radius = 0.7; material = "water"; },
{ type = "sphere"; position = { x = 2.5; y = 0.7; z = 3.0; }; radius = 0.7; material = "water"; },
{ type = "sphere"; position = { x = 0.0; y = 10.0; z = -15.0; }; radius = 5.0; material = "gold"; }
);
```


---

## 📊 Paramètres principaux des formes

| Paramètre      | Type      | Description |
|---------------|-----------|------------|
| `type`        | string    | Type de forme géométrique. Exemples : `sphere`, `cube`, `plane`. |
| `position`    | group     | Coordonnées dans l’espace `{ x, y, z }`. |
| `radius`      | float     | Rayon pour les sphères. Valeur positive pour une sphère normale, négative peut inverser la surface. |
| `material`    | string    | Référence à l’`id` d’un matériau défini dans la section matériaux. |

---

## 🔹 Exemples unitaires simples

### 1. Sol sphérique
```yaml
{ type = "sphere"; position = { x = 0.0; y = -1000.0; z = 0.0; }; radius = 1000.0; material = "ground"; }
```
- Grande sphère sous la scène simulant un sol infini.

### 2. Sphère transparente
```yaml
{ type = "sphere"; position = { x = 0.0; y = 1.5; z = 0.0; }; radius = 1.5; material = "glass"; }
```
- Sphère en verre avec réfraction.

### 3. Sphère métallique
```yaml
{ type = "sphere"; position = { x = -4.0; y = 1.0; z = -2.0; }; radius = 1.0; material = "gold"; }
```
- Sphère dorée, brillante et réfléchissante.

### 4. Sphère colorée
```yaml
{ type = "sphere"; position = { x = 4.0; y = 1.0; z = -2.0; }; radius = 1.0; material = "red_plastic"; }
```
- Sphère rouge, idéale pour du plastique ou peinture.

### 5. Sphère d’eau
```yaml
{ type = "sphere"; position = { x = -2.5; y = 0.7; z = 3.0; }; radius = 0.7; material = "water"; }
```
- Sphère simulant l’eau, transparente avec indice de réfraction correct.

---

## ✅ Astuces

- **Toujours vérifier le `radius`** : une valeur négative peut inverser la sphère (utile pour certaines simulations de lumière interne).  
- Assurez-vous que chaque forme référence un **matériau existant** pour éviter les erreurs d’affichage.  
- Positionnez vos formes de manière à éviter les collisions inattendues, sauf si l’effet est voulu.  
- Les grandes sphères peuvent être utilisées pour simuler le sol ou l’environnement lointain.  

Cette configuration permet de construire des scènes complexes en combinant des sphères de tailles, matériaux et positions variées.