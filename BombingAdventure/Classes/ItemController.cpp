//
// Created by Brando Zhang on 2018/5/19.
//

#include "GameScene.h"
#include "ItemController.h"

bool ItemController::init() {
    /* Get current time to initialize a random seed     */
    timeval now;
    gettimeofday(&now, NULL);
    auto seed = (unsigned) (now.tv_sec * 1000 + now.tv_usec / 1000);
    srand(seed);

    /* Enable update    */
    this->scheduleUpdate();
    return true;
}
void ItemController::update(float delta) {

	int i = 0;
	for (auto iter = current_item_vector.begin(); iter != current_item_vector.end();) {
        /* Check whether this item is picked by the monitored player    */
        if (is_picked(*iter)) {
            monitored_player->pick_item(*iter);
            current_item_vector.at(i)->removeAllChildren();
			current_item_vector.at(i)->removeFromParent();
            iter = current_item_vector.erase(i);
			log("Item is removed.");
		} else {
			iter++;
		}
		i++;
    }
}
void ItemController::create_item() {
    for (int i = 0; i < NUM_ITEMS; i++) {

        /* Create NUM_ITEMS items */
        Item * item = Item::create();
        item->reset_position();
		while (cannot_be_placed(item->getPosition())) {
			item->reset_position();
		}

        /* Make the item under the control of ItemController  */
        this->addChild(item);
        current_item_vector.pushBack(item);
    }
}

void ItemController::bind_player(Player *player) {
    monitored_player = player;
}

void ItemController::bind_bricks_layer(TMXLayer * bricks)
{
	_bricks = bricks;
}

void ItemController::bind_destructable_layer(TMXLayer * destructable)
{
	_destructable = destructable;
}

bool ItemController::cannot_be_placed(Vec2 pos)
{
	int firstGID_brk = _bricks->getTileSet()->_firstGid;
	int firstGID_des = _destructable->getTileSet()->_firstGid;

	Vec2 tile_coord = Vec2(pos.x / TILE_SIZE.width, (MAP_SIZE.height * TILE_SIZE.height - pos.y) / TILE_SIZE.height);
	int GID_brk = _bricks->getTileGIDAt(tile_coord);
	int GID_des = _destructable->getTileGIDAt(tile_coord);

	return (GID_brk - firstGID_brk >= 0 && GID_des - firstGID_des < 0);
}

bool ItemController::is_picked(Item *item) {
    /* Get the bounding box of the player   */
    Rect player_bounding_box = monitored_player->getBoundingBox();
    Vec2 item_position = item->getPosition();
    /* Check whether the player and the item has intersection   */
    item->is_picked = player_bounding_box.containsPoint(item_position);
    return item->is_picked;
}