
#include "entity.h"

void SpriteEntity::update(float seconds) {

	m().update(seconds);

	a().update(seconds);
	if (!a().isPaused()) {
		s().setFrame(a().getFrame());
	}
}