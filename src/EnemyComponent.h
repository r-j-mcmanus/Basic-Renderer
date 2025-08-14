#pragma once

#include <memory>

#include "Material.h"
#include "Component.h"



class EnemyComponent : public Component {

public:
    EnemyComponent(float maxHealth, float currentHealth, float moveSpeed, float attackDamage ):
        maxHealth(maxHealth), currentHealth(currentHealth), moveSpeed(moveSpeed), attackDamage(attackDamage){};

private:
    // Stats
    float maxHealth;
    float currentHealth;
    float moveSpeed;
    float attackDamage;

    // State
    bool isDead = false;

    // Internal hooks
    void onDeath() {
        // Trigger death animation, notify scene, etc.
    }
};