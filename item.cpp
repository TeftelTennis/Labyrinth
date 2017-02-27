#include "item.h"


Item::Item() {
    //image = new Texture();
    name = "empty";
    itemType = ITEM_TYPE_EMPTY;
    type = TYPE_ITEM;
    tostring = []() {
            return "item: " + name;
        };
    infoWindow = [](int windowID) {
        GUILayout.Label("--EMPTY--");
    };
    hitPlayer = [](Player player, Labyrinth field) {};
    use = [](Plyer player, Labyrinth field) {};
    hitWall = [](Direction direction) {return false;};
}

ArmorPiercingBullet::ArmorPiercingBullet() {
    super();
}

Flower::Flower() {
    super();
}

Bullet::Bullet() {
    //image = AssetDatabase.LoadAssetAtPath("Assets/Textures/18040.jpg", typeof(Texture)) as Texture;
    itemType = ITEM_TYPE_BULLET;
    name = "bullet";
    hitPlayer = [](Player player, Labyrinth field) {
        player.life--;
        player.alive = player.life > 0;
    };
}

Key::Key() {
    itemType = ITEM_TYPE_KEY;
    name = "key";
}
