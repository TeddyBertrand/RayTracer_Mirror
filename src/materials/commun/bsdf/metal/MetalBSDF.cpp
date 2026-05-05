#include "MetalBSDF.hpp"

namespace Raytracer {

bool MetalBSDF::scatter(const Ray& r_in,
                        const HitRecord& hit,
                        Color& attenuation,
                        Ray& scattered) const {
    
    // 1. Calculer la direction de réflexion pure (I - 2 * dot(I, N) * N)
    // On normalise r_in.direction() pour avoir un calcul propre
    Vector3D reflected = Vector3D::reflect(r_in.direction().normalized(), hit.normal);

    // 2. Ajouter le "Fuzz" (le flou)
    // On ajoute un petit vecteur aléatoire au bout du vecteur réfléchi
    Vector3D scatter_direction = reflected + (_fuzz * Vector3D::getRandomUnitVector());

    // 3. Préparer le rayon sortant
    scattered = Ray(hit.point, scatter_direction.normalized(), RayType::REFLECT);
    
    // 4. L'atténuation est la couleur du métal
    attenuation = _albedo_texture->value(hit.u, hit.v);

    // 5. Vérifier que le rayon rebondit bien vers l'extérieur (pas à travers la surface)
    return (scattered.direction().dot(hit.normal) > 0);
}

}