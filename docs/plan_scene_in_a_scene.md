# 🚀 Plan d'implémentation : Recursive Scene Importing (v2)

Ce document détaille la stratégie optimisée pour permettre l'importation de fichiers `.scene` dans d'autres fichiers, en garantissant performance, robustesse et flexibilité hiérarchique.

---

## 🏗️ 1. Les Patterns de Design engagés

| Pattern | Composant | Rôle |
| :--- | :--- | :--- |
| **Composite** | `PrimitiveGroup` | Permet de traiter une liste d'objets (scène importée) comme une seule `IPrimitive`. |
| **Decorator** | `Entity` | (Existant) Applique les transformations (Matrix) au groupe importé. |
| **Flyweight** | `SceneCache` | Évite de parser et de dupliquer la géométrie si le même fichier est importé plusieurs fois. |
| **Mediator** | `SceneManager` | Orchestre la fusion des ressources (Lumières, Matériaux) et résout les namespaces. |
| **Builder** | `SceneBuilder` | Construit la scène finale en utilisant le Parser récursif. |

---

## 🛠️ 2. Phase 1 : Le Composite Optimisé (`PrimitiveGroup`)

Créer une classe qui hérite de `IPrimitive` pour regrouper les objets importés.

- **Data** : `std::vector<std::shared_ptr<IPrimitive>> _children`.
- **Hit()** : 
    - Test rapide de l'AABB globale du groupe.
    - Si touchée, boucle sur les enfants pour trouver le `HitRecord` le plus proche.
    - *Optimisation* : Si le nombre d'enfants dépasse un seuil, construire un mini-BVH interne.
- **BoundingBox()** : Retourne une `AABB` englobante pré-calculée.

---

## 🏛️ 3. Phase 2 : Gestion des Ressources & Namespacing

Le Mediator gère l'isolation et la fusion des ressources.

- **Gestion des Matériaux** : 
    - **Isolation** : Résoudre les conflits en préfixant les matériaux (ex: `car::red`).
    - **Overriding** : Permettre à la scène parente de substituer un matériau de la sous-scène par un sien lors de l'import (ex: `remap: ("old_mat" -> "new_mat")`).
- **Fusion des Lumières** : 
    - Extraire les lumières d'un import.
    - Appliquer la transformation complète de l'import à chaque lumière (position pour les PointLights, direction pour les DirectionalLights).
    - Les ajouter au registre global des lumières.

---

## 📜 4. Phase 3 : Le Parser Récursif & Sécurité

Modifier la logique de lecture pour supporter l'arbre de scène.

- **Protection contre la récursion** : Maintenir un `std::set` des chemins de fichiers en cours de lecture pour détecter et bloquer les imports circulaires (A -> B -> A).
- **SceneCache** : Avant de parser un fichier, vérifier s'il existe déjà dans le cache pour réutiliser le `PrimitiveGroup` (Flyweight).
- **Logique d'Import** :
    1. Si `type = "import"`, lire le `path` (gérer les chemins relatifs).
    2. Récupérer ou créer le `SceneContext` (Groupe + Lumières + Matériaux).
    3. Envoyer ce contexte au **Mediator** pour intégration et application des remaps.

---

## 📐 5. Phase 4 : Transformation & Hiérarchie

- **Shapes** : Envelopper le `PrimitiveGroup` (issu du cache ou neuf) dans une `Entity`. Cela permet d'appliquer des transformations locales sans modifier la géométrie partagée.
- **Lights** : Calculer la matrice de transformation mondiale au moment de l'import pour placer les lumières correctement dès le début.
- **Caméras** : Par défaut, ignorer les caméras définies dans les scènes importées (seule la caméra de la scène racine est conservée).
