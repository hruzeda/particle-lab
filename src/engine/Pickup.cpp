#include "../../inc/engine/Pickup.h"

#include <SFML/Graphics/RenderTarget.hpp>

#include "../../inc/engine/Category.h"
#include "../../inc/engine/CommandQueue.h"
#include "../../inc/engine/DataTables.h"
#include "../../inc/engine/ResourceHolder.h"
#include "../../inc/engine/Utility.h"

namespace {
const std::vector<PickupData> Table = initializePickupData();
}

Pickup::Pickup(Type type, const TextureHolder& textures)
    : Entity(1),
      mType(type),
      mSprite(textures.get(Table[type].texture), Table[type].textureRect) {
  centerOrigin(mSprite);
}

unsigned int Pickup::getCategory() const { return Category::Pickup; }

sf::FloatRect Pickup::getBoundingRect() const {
  return getWorldTransform().transformRect(mSprite.getGlobalBounds());
}

void Pickup::apply(Aircraft& player) const { Table[mType].action(player); }

void Pickup::drawCurrent(sf::RenderTarget& target,
                         sf::RenderStates states) const {
  target.draw(mSprite, states);
}
