
#include <algorithm>
#include "collision.h"

//Checks if the rotation of a transformable is small enough to ignore rotation
//The .getRotation() function returns it in the range 0 to 360 degrees.
const bool isAxisAligned(float rotation) {
	while (rotation > 90)
		rotation -= 90;
	return rotation <= 2 || rotation >= 88;
}

//Functions for checking collisions

//Maths functions

bool linesIntersect(const sf::Vector2f a1, const sf::Vector2f a2, const sf::Vector2f b1, const sf::Vector2f b2) {

	//Note: This function was from an earlier project, I can't remember how this works except that its from
	//some rearrangement of the formula x=(m2-m1)/(c1-c2), where m = gradient and c = y-intersect.

	//(yB2–yB1)(xA2–xA1)–(xB2–xB1)(yA2–yA1)
	double denom = (b2.y - b1.y)*(a2.x - a1.x) - (b2.x - b1.x)*(a2.y - a1.y);
	if (denom == 0) //Lines are parallel
		return false;

	//ua=(xB2–xB1)(yA1–yB1)–(yB2–yB1)(xA1–xB1) / denom
	double xFraction = ((b2.x - b1.x)*(a1.y - b1.y) - (b2.y - b1.y)*(a1.x - b1.x)) / denom;
	if (xFraction < 0 || xFraction>1)
		return false;

	//ub=(xA2–xA1)(yA1–yB1)–(yA2–yA1)(xA1–xB1) / denom
	double yFraction = ((a2.x - a1.x)*(a1.y - b1.y) - (a2.y - a1.y)*(a1.x - b1.x)) / denom;
	if (yFraction < 0 || yFraction>1)
		return false;

	return true;
}

bool lineIntersectsCircle(const sf::Vector2f a, const sf::Vector2f b, const sf::Vector2f c, float r) {
	//a,b = ends of line, c = circle pos, r = radius
	//Uses vector equation of line, pos = start + lambda*direction, where direction=end-start
	//Equation from lamda comes from finding the dot product of the line direction and line from
	//the circle to general point on line, then equating this to zero (which means lines are perpendicular)
	//then rearranging to find lambda.
	//Lambda < 0 means that the perpendicular from the circle centre intersects the line before the start.
	//Lambda > 1 means that the perpendicular from the circle centre intersects the line after the end.
	//0 < Lambda < 1 means that the perpendicular lies on the line, so need to find the perpendicular dist.

	double lambda = ((c.x - a.x)*(b.x - a.x) + (c.y - a.y)*(b.y - a.y)) / (pow(b.x - a.x, 2) + pow(b.y - a.y, 2));
	double dist = 0;

	if (lambda < 0) {
		dist = hypot(c.x - a.x, c.y - a.y);
	}
	else if (lambda > 1) {
		dist = hypot(c.x - b.x, c.y - b.y);
	}
	else {
		dist = hypot(c.x - (a.x + lambda * (b.x - a.x)), c.y - (a.y + lambda * (b.y - a.y)));
	}

	return dist <= r;
}

std::vector<sf::Vector2f> rectToPolygon(const sf::FloatRect rect) {
	std::vector<sf::Vector2f> vertices;
	vertices.push_back(sf::Vector2f(rect.left, rect.top));
	vertices.push_back(sf::Vector2f(rect.left + rect.width, rect.top));
	vertices.push_back(sf::Vector2f(rect.left + rect.width, rect.top + rect.height));
	vertices.push_back(sf::Vector2f(rect.left, rect.top + rect.height));

	return vertices;
}

//Functions that take the transformables and hitboxes

//These functions check for collisions between circles, rects, polygons.
//They take the hitbox and the transform (which comes from Transformable::getTransform())
//The transform is used to get the position of the hitboxes.
//If a transform involves a rotation, then this results in:
//no change for circles
//for rectangles, the bounding rectangle for the rotated rectangle is used
//for polygons, it rotates the polygon.
//Therefore, if you need accurate collision detection with rotation, the hitbox must be of polygon type.

const bool collideCC(const sf::Transform& at, const float ar, const sf::Transform& bt, const float br) {
	sf::Vector2f displacement = bt.transformPoint(0, 0) - at.transformPoint(0, 0);
	float distance = hypot(displacement.x, displacement.y);
	if (distance <= (ar + br)) {
		return true;
	}
	else {
		return false;
	}
}

//Doesn't take into account rotation, only called if !checkRotatedRects or rect is axis aligned
const bool collideCR(const sf::Transform& ct, const float radius, const sf::Transform& rt, const sf::FloatRect roughRect) {
	sf::FloatRect rect = rt.transformRect(roughRect);
	sf::Vector2f circlePos = ct.transformPoint(0, 0);

	float xDist = abs((rect.left + rect.width / 2) - circlePos.x);
	float yDist = abs((rect.top + rect.height / 2) - circlePos.y);

	if (xDist <= rect.width / 2 || yDist <= rect.height / 2) {
		return true;
	}

	float distToCorner = hypot(xDist - rect.width / 2, yDist - rect.height / 2);

	return distToCorner <= radius;
}

const bool collideCP(const sf::Transform& ct, const float radius, const sf::Transform& pt, const std::vector<sf::Vector2f>& vertices) {
	if (vertices.size() == 0)
		return false;

	sf::Vector2f a, b;
	sf::Vector2f c = ct.transformPoint(0, 0);

	b = pt.transformPoint(vertices[0]);
	for (std::vector<sf::Vector2f>::const_iterator iter = vertices.begin() + 1; iter != vertices.end() + 1; iter++) {
		a = b;
		b = pt.transformPoint(iter != vertices.end() ? *iter : vertices[0]);
		if (lineIntersectsCircle(a, b, c, radius)) {
			return true;
		}
	}
	return false;
}

const bool collidePP(const sf::Transform& at, const std::vector<sf::Vector2f>& av, const sf::Transform& bt, const std::vector<sf::Vector2f>& bv) {
	if (av.size() == 0 || bv.size() == 0)
		return false;

	sf::Vector2f a1, a2, b1, b2;

	a2 = at.transformPoint(av[0]);
	for (std::vector<sf::Vector2f>::const_iterator aIter = av.begin() + 1; aIter != av.end() + 1; aIter++) {
		a1 = a2;
		a2 = at.transformPoint(aIter != av.end() ? *aIter : av[0]);
		b2 = bt.transformPoint(bv[0]);
		for (std::vector<sf::Vector2f>::const_iterator bIter = bv.begin() + 1; bIter != bv.end() + 1; bIter++) {
			b1 = b2;
			b2 = bt.transformPoint(bIter != bv.end() ? *bIter : bv[0]);

			if (linesIntersect(a1, a2, b1, b2)) {
				return true;
			}
		}
	}
	return false;
}

const bool collideRP(const sf::Transform& rt, const sf::FloatRect& rect, const sf::Transform& pt, const std::vector<sf::Vector2f>& vertices) {
	std::vector<sf::Vector2f> rectVertices = rectToPolygon(rect);
	return collidePP(rt, rectVertices, pt, vertices);
}

//if checkRotatedRects (which defaults to true) is false, then the transformed roughBounds are used (which are the bounds of the rotated rect) for checking
//collisions with rectangles, except for the RP collision which always takes into account rotation since it doesn't change much.

const bool collide(const sf::Transformable& a, Hitbox& ah, const sf::Transformable& b, Hitbox& bh) {
	if (ah.getType() == HitboxType::NONE || bh.getType() == HitboxType::NONE) {
		return false;
	}

	sf::Transform at = getHitboxTransform(a, ah);
	sf::Transform bt = getHitboxTransform(b, bh);

	sf::FloatRect aRoughRect = at.transformRect(ah.getRoughBounds());
	sf::FloatRect bRoughRect = bt.transformRect(bh.getRoughBounds());

	if (aRoughRect.intersects(bRoughRect)) {
		if (ah.getType() == HitboxType::POLYGON_RECT) {
			ah.updatePolygonRect(a.getRotation(), a.getScale());
			at = getHitboxTransform(a, ah, true);
			aRoughRect = at.transformRect(ah.getExactBounds());
		}
		if (bh.getType() == HitboxType::POLYGON_RECT) {
			bh.updatePolygonRect(b.getRotation(), b.getScale());
			bt = getHitboxTransform(b, bh, true);
			bRoughRect = bt.transformRect(bh.getExactBounds());
		}
		if (ah.getType() == HitboxType::POLYGON_RECT || bh.getType() == HitboxType::POLYGON_RECT) {
			if (!aRoughRect.intersects(bRoughRect))
				return false;
		}
		switch (ah.getType()) {
		case HitboxType::CIRCLE:
			switch (bh.getType()) {
			case HitboxType::CIRCLE:
				return collideCC(at, ah.getRadius()*maxAbsScale(a.getScale()), bt, bh.getRadius()*maxAbsScale(b.getScale()));
			case HitboxType::RECT:
				return collideCR(at, ah.getRadius()*maxAbsScale(a.getScale()), bt, bh.getRoughBounds());
			case HitboxType::POLYGON:
				return collideCP(at, ah.getRadius()*maxAbsScale(a.getScale()), bt, bh.getVertices());
			case HitboxType::POLYGON_RECT:
				return collideCR(at, ah.getRadius()*maxAbsScale(a.getScale()), bt, bh.getExactBounds());
			}
			break;
		case HitboxType::RECT:
			switch (bh.getType()) {
			case HitboxType::CIRCLE:
				return collideCR(bt, bh.getRadius()*maxAbsScale(b.getScale()), at, ah.getRoughBounds());
			case HitboxType::RECT:
				return true;
			case HitboxType::POLYGON:
				return collideRP(at, ah.getRoughBounds(), bt, bh.getVertices());
			case HitboxType::POLYGON_RECT:
				return true;
			}
			break;
		case HitboxType::POLYGON:
			switch (bh.getType()) {
			case HitboxType::CIRCLE:
				return collideCP(bt, bh.getRadius()*maxAbsScale(b.getScale()), at, ah.getVertices());
			case HitboxType::RECT:
				return collideRP(bt, bh.getRoughBounds(), at, ah.getVertices());
			case HitboxType::POLYGON:
				return collidePP(at, ah.getVertices(), bt, bh.getVertices());
			case HitboxType::POLYGON_RECT:
				return collideRP(bt, bh.getExactBounds(), at, ah.getVertices());
			}
			break;
		case HitboxType::POLYGON_RECT:
			switch (bh.getType()) {
			case HitboxType::CIRCLE:
				return collideCR(bt, bh.getRadius()*maxAbsScale(b.getScale()), at, ah.getExactBounds());
			case HitboxType::RECT:
				return true;
			case HitboxType::POLYGON:
				return collideRP(at, ah.getExactBounds(), bt, bh.getVertices());
			case HitboxType::POLYGON_RECT:
				return true;
			}
			break;
		}
	}

	return false;
}

const bool collideTileMapV(const TileMap& tilemap, const sf::Transform& transform, const sf::FloatRect& rect, const std::vector<sf::Vector2f>& vertices) {
	sf::FloatRect roughBounds = transform.transformRect(rect);
	sf::Vector2f posA = sf::Vector2f(roughBounds.left, roughBounds.top) - tilemap.getPosition();
	sf::Vector2f posB = posA + sf::Vector2f(roughBounds.width, roughBounds.height);
	int xPosA = floor(posA.x / tilemap.getTileMapData().getTileSize().x);
	int yPosA = floor(posA.y / tilemap.getTileMapData().getTileSize().y);
	int xPosB = floor(posB.x / tilemap.getTileMapData().getTileSize().x);
	int yPosB = floor(posB.y / tilemap.getTileMapData().getTileSize().y);

	sf::FloatRect tileRect;
	sf::Transform tilemapTransform = (sf::Transform()).translate(tilemap.getPosition());

	for (int y = yPosA; y != yPosB + 1; y++) {
		for (int x = xPosA; x != xPosB + 1; x++) {
			if (tilemap.getTileMapData().isSolid(x, y)) {
				tileRect = sf::FloatRect((float)x*tilemap.getTileMapData().getTileSize().x, (float)y*tilemap.getTileMapData().getTileSize().y,
					tilemap.getTileMapData().getTileSize().x, tilemap.getTileMapData().getTileSize().y);
				if (collideRP(tilemapTransform, tileRect, transform, vertices)) {
					return true;
				}
			}
		}
	}
	return false;
}

const bool collideTileMapC(const TileMap& tilemap, const sf::Transform& transform, const sf::FloatRect& rect, const float radius) {
	sf::FloatRect roughBounds = transform.transformRect(rect);
	sf::Vector2f posA = sf::Vector2f(roughBounds.left, roughBounds.top) - tilemap.getPosition();
	sf::Vector2f posB = posA + sf::Vector2f(roughBounds.width, roughBounds.height);
	int xPosA = floor(posA.x / tilemap.getTileMapData().getTileSize().x);
	int yPosA = floor(posA.y / tilemap.getTileMapData().getTileSize().y);
	int xPosB = floor(posB.x / tilemap.getTileMapData().getTileSize().x);
	int yPosB = floor(posB.y / tilemap.getTileMapData().getTileSize().y);

	sf::FloatRect tileRect;
	sf::Transform tilemapTransform = (sf::Transform()).translate(tilemap.getPosition());

	for (int y = yPosA; y != yPosB + 1; y++) {
		for (int x = xPosA; x != xPosB + 1; x++) {
			if (tilemap.getTileMapData().isSolid(x, y)) {
				tileRect = sf::FloatRect((float)x*tilemap.getTileMapData().getTileSize().x, (float)y*tilemap.getTileMapData().getTileSize().y,
					tilemap.getTileMapData().getTileSize().x, tilemap.getTileMapData().getTileSize().y);
				if (collideCR(transform, radius, tilemapTransform, tileRect)) {
					return true;
				}
			}
		}
	}
	return false;
}

const bool collideTileMapR(const TileMap& tilemap, const sf::Transform& transform, const sf::FloatRect& rect) {
	//Find the position of the top,left and bottom,right corner of the roughBounds in the tilemap.
	sf::FloatRect roughBounds = transform.transformRect(rect);
	sf::Vector2f posA = sf::Vector2f(roughBounds.left, roughBounds.top) - tilemap.getPosition();
	sf::Vector2f posB = posA + sf::Vector2f(roughBounds.width, roughBounds.height);
	int xPosA = floor(posA.x / tilemap.getTileMapData().getTileSize().x);
	int yPosA = floor(posA.y / tilemap.getTileMapData().getTileSize().y);
	int xPosB = floor(posB.x / tilemap.getTileMapData().getTileSize().x);
	int yPosB = floor(posB.y / tilemap.getTileMapData().getTileSize().y);

	for (int y = yPosA; y != yPosB + 1; y++) {
		for (int x = xPosA; x != xPosB + 1; x++) {
			if (tilemap.getTileMapData().isSolid(x, y)) {
				return true;
			}
		}
	}
	return false;
}

const bool collideTileMap(const TileMap& tilemap, const sf::Transformable& transformable, Hitbox& hitbox) {
	//Find the position of the top,left and bottom,right corner of the roughBounds in the tilemap.
	sf::Transform transform = getHitboxTransform(transformable, hitbox);

	if (!collideTileMapR(tilemap, transform, hitbox.getRoughBounds())) {
		return false;
	}

	switch (hitbox.getType()) {
	case HitboxType::CIRCLE:
		return collideTileMapC(tilemap, transform, hitbox.getRoughBounds(), hitbox.getRadius()*maxAbsScale(transformable.getScale()));
	case HitboxType::RECT:
		return true;
	case HitboxType::POLYGON:
		return collideTileMapV(tilemap, transform, hitbox.getRoughBounds(), hitbox.getVertices());
	case HitboxType::POLYGON_RECT:
		hitbox.updatePolygonRect(transformable.getRotation(), transformable.getScale());
		transform = getHitboxTransform(transformable, hitbox, true);
		return collideTileMapR(tilemap, transform, hitbox.getExactBounds());
	}
	return false;
}